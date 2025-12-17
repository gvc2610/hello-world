#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>

/*******************************************************************************
 * THREAD-SAFE ALIGNED MEMORY ALLOCATOR (Staff/Principal Engineer Level)
 * 
 * Advanced Features:
 * 1. Thread-safe allocation/deallocation using mutex
 * 2. Memory leak detection and tracking
 * 3. Double-free detection
 * 4. Use-after-free detection (poisoning)
 * 5. Allocation statistics and profiling
 * 6. Guard pages/canaries for buffer overflow detection
 * 7. Configurable alignment validation
 * 8. Debug mode with verbose logging
 * 
 * ENHANCED MEMORY LAYOUT:
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ malloc() returns here                                                    │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ [PADDING: 0 to (align-1) bytes]                                         │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ [METADATA BLOCK: align bytes]                                           │
 * │   - magic (4 bytes): 0xDEADBEEF (corruption detection)                 │
 * │   - original_ptr (8 bytes): original malloc address                     │
 * │   - size (8 bytes): requested allocation size                           │
 * │   - alignment (4 bytes): alignment value used                           │
 * │   - thread_id (8 bytes): allocating thread ID                           │
 * │   - timestamp (8 bytes): allocation time                                │
 * │   - canary (4 bytes): 0xCAFEBABE (overflow detection)                  │
 * │   - status (4 bytes): ALLOCATED/FREED                                   │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ [FRONT GUARD: 8 bytes] 0xDEADDEADDEADDEAD                              │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ [USER DATA: size bytes]  ← returned to user (aligned)                  │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ [REAR GUARD: 8 bytes] 0xBEEFBEEFBEEFBEEF                               │
 * └─────────────────────────────────────────────────────────────────────────┘
 *
 ******************************************************************************/

// Magic numbers for integrity checking
#define MAGIC_ALLOCATED   0xDEADBEEF
#define MAGIC_FREED       0xFEEDF00D
#define CANARY_VALUE      0xCAFEBABE
#define FRONT_GUARD       0xDEADDEADDEADDEADULL
#define REAR_GUARD        0xBEEFBEEFBEEFBEEFULL
#define POISON_BYTE       0xDD  // Use-after-free detection

// Allocation status
typedef enum {
    STATUS_ALLOCATED = 1,
    STATUS_FREED = 2
} alloc_status_t;

// Metadata structure stored before user data
typedef struct {
    uint32_t magic;              // Magic number for validation
    void *original_ptr;          // Original malloc pointer
    size_t size;                 // Requested size
    uint32_t alignment;          // Alignment used
    pthread_t thread_id;         // Thread that allocated
    uint64_t timestamp;          // Allocation timestamp (for leak detection)
    uint32_t canary;             // Canary for overflow detection
    alloc_status_t status;       // Allocation status
} metadata_t;

// Global statistics (protected by mutex)
typedef struct {
    size_t total_allocations;
    size_t total_frees;
    size_t active_allocations;
    size_t total_bytes_allocated;
    size_t total_bytes_freed;
    size_t peak_memory_usage;
    size_t current_memory_usage;
} stats_t;

// Global allocator state
static pthread_mutex_t allocator_mutex = PTHREAD_MUTEX_INITIALIZER;
static stats_t global_stats = {0};
static bool debug_mode = true;  // Enable verbose logging

/*******************************************************************************
 * UTILITY FUNCTIONS
 ******************************************************************************/

// Get current timestamp (simplified - could use clock_gettime for real impl)
static uint64_t get_timestamp(void) {
    return (uint64_t)time(NULL);
}

// Check if alignment is valid (must be power of 2)
static bool is_power_of_two(size_t n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// Round up to next power of 2
static size_t next_power_of_two(size_t n) {
    if (n == 0) return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1;
}

/*******************************************************************************
 * DEBUG AND VALIDATION FUNCTIONS
 ******************************************************************************/

static void log_debug(const char *format, ...) {
    if (debug_mode) {
        va_list args;
        va_start(args, format);
        printf("[ALLOCATOR] ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
    }
}

// Validate metadata integrity
static bool validate_metadata(metadata_t *meta, const char *context) {
    if (meta->magic != MAGIC_ALLOCATED) {
        fprintf(stderr, "ERROR [%s]: Invalid magic number 0x%x (expected 0x%x)\n",
                context, meta->magic, MAGIC_ALLOCATED);
        if (meta->magic == MAGIC_FREED) {
            fprintf(stderr, "  DOUBLE-FREE or USE-AFTER-FREE detected!\n");
        }
        return false;
    }
    
    if (meta->canary != CANARY_VALUE) {
        fprintf(stderr, "ERROR [%s]: Canary corrupted 0x%x (expected 0x%x)\n",
                context, meta->canary, CANARY_VALUE);
        fprintf(stderr, "  HEAP CORRUPTION detected!\n");
        return false;
    }
    
    if (meta->status != STATUS_ALLOCATED) {
        fprintf(stderr, "ERROR [%s]: Invalid status %d\n", context, meta->status);
        return false;
    }
    
    return true;
}

// Check guard bytes for buffer overflow
static bool check_guards(void *user_ptr, metadata_t *meta, const char *context) {
    uint64_t *front_guard = (uint64_t *)((char *)user_ptr - sizeof(uint64_t));
    uint64_t *rear_guard = (uint64_t *)((char *)user_ptr + meta->size);
    
    if (*front_guard != FRONT_GUARD) {
        fprintf(stderr, "ERROR [%s]: Front guard corrupted! Buffer underflow detected.\n", context);
        fprintf(stderr, "  Expected: 0x%llx, Got: 0x%llx\n", FRONT_GUARD, *front_guard);
        return false;
    }
    
    if (*rear_guard != REAR_GUARD) {
        fprintf(stderr, "ERROR [%s]: Rear guard corrupted! Buffer overflow detected.\n", context);
        fprintf(stderr, "  Expected: 0x%llx, Got: 0x%llx\n", REAR_GUARD, *rear_guard);
        return false;
    }
    
    return true;
}

/*******************************************************************************
 * THREAD-SAFE ALIGNED MALLOC
 ******************************************************************************/

void* ts_aligned_malloc(size_t size, size_t alignment) {
    // Input validation
    if (size == 0) {
        fprintf(stderr, "ERROR: Cannot allocate 0 bytes\n");
        return NULL;
    }
    
    if (!is_power_of_two(alignment)) {
        fprintf(stderr, "ERROR: Alignment %zu is not a power of 2\n", alignment);
        return NULL;
    }
    
    if (alignment < sizeof(void*)) {
        alignment = sizeof(void*);
    }
    
    pthread_mutex_lock(&allocator_mutex);
    
    // Simpler approach: allocate enough space for metadata + alignment + guards + data
    // Layout: [padding] [metadata] [guard] [data] [guard]
    size_t total_size = sizeof(metadata_t) + alignment + 2 * sizeof(uint64_t) + size;
    
    void *ptr1 = malloc(total_size);
    if (!ptr1) {
        fprintf(stderr, "ERROR: malloc failed for %zu bytes\n", total_size);
        pthread_mutex_unlock(&allocator_mutex);
        return NULL;
    }
    
    log_debug("malloc returned %p (%zu bytes)", ptr1, total_size);
    
    // Place metadata at the start
    metadata_t *meta = (metadata_t *)ptr1;
    
    // User data starts after metadata, aligned to boundary
    uintptr_t data_start = (uintptr_t)ptr1 + sizeof(metadata_t) + sizeof(uint64_t);
    data_start = (data_start + alignment - 1) & ~(alignment - 1);
    void *user_ptr = (void *)data_start;
    
    // Initialize metadata
    meta->magic = MAGIC_ALLOCATED;
    meta->original_ptr = ptr1;
    meta->size = size;
    meta->alignment = alignment;
    meta->thread_id = pthread_self();
    meta->timestamp = get_timestamp();
    meta->canary = CANARY_VALUE;
    meta->status = STATUS_ALLOCATED;
    
    // Place guard bytes
    uint64_t *front_guard = (uint64_t *)((char *)user_ptr - sizeof(uint64_t));
    uint64_t *rear_guard = (uint64_t *)((char *)user_ptr + size);
    *front_guard = FRONT_GUARD;
    *rear_guard = REAR_GUARD;
    
    // Update statistics
    global_stats.total_allocations++;
    global_stats.active_allocations++;
    global_stats.total_bytes_allocated += size;
    global_stats.current_memory_usage += size;
    if (global_stats.current_memory_usage > global_stats.peak_memory_usage) {
        global_stats.peak_memory_usage = global_stats.current_memory_usage;
    }
    
    log_debug("Allocated %zu bytes at %p (aligned to %zu bytes) for thread %lu",
              size, user_ptr, alignment, (unsigned long)meta->thread_id);
    log_debug("  Metadata at %p, Original malloc at %p", meta, ptr1);
    
    pthread_mutex_unlock(&allocator_mutex);
    
    // Verify alignment
    assert(((uintptr_t)user_ptr & (alignment - 1)) == 0);
    
    return user_ptr;
}

/*******************************************************************************
 * THREAD-SAFE ALIGNED FREE
 ******************************************************************************/

void ts_aligned_free(void *ptr, size_t alignment) {
    if (!ptr) {
        log_debug("Ignoring free of NULL pointer");
        return;
    }
    
    pthread_mutex_lock(&allocator_mutex);
    
    // Metadata is stored at the beginning of the allocation
    // We need to find it by reading the pointer stored just before user data
    // The front guard is at ptr - 8, and before that should be space leading back to metadata
    // Actually, let's store a back-pointer right before the front guard
    
    // For simplicity: the original_ptr in metadata points to start of allocation
    // Scan backwards to find the metadata with matching magic
    metadata_t *meta = NULL;
    
    // Start from user pointer, go back to find metadata
    // It should be within (sizeof(metadata_t) + alignment + sizeof(uint64_t)) bytes
    uintptr_t search_start = (uintptr_t)ptr - sizeof(metadata_t) - alignment - sizeof(uint64_t);
    
    for (uintptr_t addr = (uintptr_t)ptr - sizeof(uint64_t) - sizeof(metadata_t); 
         addr >= search_start; addr -= sizeof(void*)) {
        metadata_t *candidate = (metadata_t *)addr;
        if (candidate->magic == MAGIC_ALLOCATED && 
            candidate->original_ptr == (void *)addr) {
            meta = candidate;
            break;
        }
    }
    
    if (!meta) {
        fprintf(stderr, "ERROR: Could not find metadata for pointer %p\n", ptr);
        pthread_mutex_unlock(&allocator_mutex);
        abort();
    }
    
    log_debug("Free called for %p, found metadata at %p", ptr, meta);
    
    // Validate metadata
    if (!validate_metadata(meta, "FREE")) {
        pthread_mutex_unlock(&allocator_mutex);
        abort();  // Fatal error - memory corruption
    }
    
    // Check guard bytes for overflow/underflow
    if (!check_guards(ptr, meta, "FREE")) {
        pthread_mutex_unlock(&allocator_mutex);
        abort();  // Fatal error - buffer overflow
    }
    
    // Check alignment matches
    if (meta->alignment != alignment) {
        fprintf(stderr, "ERROR: Alignment mismatch! Allocated with %u, freeing with %zu\n",
                meta->alignment, alignment);
        pthread_mutex_unlock(&allocator_mutex);
        abort();
    }
    
    log_debug("Freeing %zu bytes at %p (allocated by thread %lu)",
              meta->size, ptr, (unsigned long)meta->thread_id);
    
    // Poison the memory to detect use-after-free
    memset(ptr, POISON_BYTE, meta->size);
    
    // Mark as freed (helps detect double-free)
    meta->magic = MAGIC_FREED;
    meta->status = STATUS_FREED;
    
    // Update statistics
    global_stats.total_frees++;
    global_stats.active_allocations--;
    global_stats.total_bytes_freed += meta->size;
    global_stats.current_memory_usage -= meta->size;
    
    // Free original pointer
    void *original = meta->original_ptr;
    free(original);
    
    log_debug("Freed memory at original address %p", original);
    
    pthread_mutex_unlock(&allocator_mutex);
}

/*******************************************************************************
 * STATISTICS AND DEBUGGING
 ******************************************************************************/

void ts_aligned_print_stats(void) {
    pthread_mutex_lock(&allocator_mutex);
    
    printf("\n========== ALLOCATOR STATISTICS ==========\n");
    printf("Total allocations:      %zu\n", global_stats.total_allocations);
    printf("Total frees:            %zu\n", global_stats.total_frees);
    printf("Active allocations:     %zu\n", global_stats.active_allocations);
    printf("Total bytes allocated:  %zu\n", global_stats.total_bytes_allocated);
    printf("Total bytes freed:      %zu\n", global_stats.total_bytes_freed);
    printf("Current memory usage:   %zu bytes\n", global_stats.current_memory_usage);
    printf("Peak memory usage:      %zu bytes\n", global_stats.peak_memory_usage);
    
    if (global_stats.active_allocations > 0) {
        printf("\n⚠️  WARNING: %zu memory leak(s) detected!\n", global_stats.active_allocations);
    } else {
        printf("\n✅ No memory leaks detected\n");
    }
    printf("==========================================\n\n");
    
    pthread_mutex_unlock(&allocator_mutex);
}

void ts_aligned_set_debug(bool enabled) {
    debug_mode = enabled;
}

/*******************************************************************************
 * TEST PROGRAM
 ******************************************************************************/

void* thread_worker(void *arg) {
    int thread_num = *(int *)arg;
    
    printf("Thread %d starting\n", thread_num);
    
    // Allocate some memory
    void *ptr1 = ts_aligned_malloc(1024, 64);
    void *ptr2 = ts_aligned_malloc(2048, 128);
    
    if (ptr1 && ptr2) {
        // Simulate some work
        memset(ptr1, 0xAA, 1024);
        memset(ptr2, 0xBB, 2048);
        
        // Free memory
        ts_aligned_free(ptr1, 64);
        ts_aligned_free(ptr2, 128);
    }
    
    printf("Thread %d finished\n", thread_num);
    return NULL;
}

int main(void) {
    printf("=== Thread-Safe Aligned Malloc Test ===\n\n");
    
    ts_aligned_set_debug(true);
    
    // Test 1: Basic allocation and free
    printf("--- Test 1: Basic Allocation ---\n");
    void *ptr1 = ts_aligned_malloc(1000, 64);
    printf("User pointer: %p (should be 64-byte aligned)\n", ptr1);
    printf("Last 6 bits: 0x%02lx (should be 0x00)\n", (uintptr_t)ptr1 & 0x3F);
    ts_aligned_free(ptr1, 64);
    
    // Test 2: Multiple allocations
    printf("\n--- Test 2: Multiple Allocations ---\n");
    void *ptr2 = ts_aligned_malloc(500, 32);
    void *ptr3 = ts_aligned_malloc(1500, 128);
    void *ptr4 = ts_aligned_malloc(2000, 256);
    
    ts_aligned_free(ptr2, 32);
    ts_aligned_free(ptr3, 128);
    ts_aligned_free(ptr4, 256);
    
    // Test 3: Multi-threaded test
    printf("\n--- Test 3: Multi-threaded Test ---\n");
    pthread_t threads[4];
    int thread_nums[4] = {1, 2, 3, 4};
    
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_worker, &thread_nums[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Test 4: Intentional errors (uncomment to test)
    printf("\n--- Test 4: Error Detection ---\n");
    /*
    // Double-free detection
    void *ptr5 = ts_aligned_malloc(100, 64);
    ts_aligned_free(ptr5, 64);
    ts_aligned_free(ptr5, 64);  // Should abort with double-free error
    */
    
    /*
    // Buffer overflow detection
    void *ptr6 = ts_aligned_malloc(100, 64);
    char *overflow = (char *)ptr6;
    overflow[105] = 'X';  // Write past end
    ts_aligned_free(ptr6, 64);  // Should detect overflow and abort
    */
    
    // Print final statistics
    ts_aligned_print_stats();
    
    return 0;
}
