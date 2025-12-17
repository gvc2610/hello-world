#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>

/*******************************************************************************
 * THREAD-SAFE ALIGNED MALLOC (Bare Bones / Interview Version)
 * 
 * This version implements the core features of a thread-safe aligned malloc:
 * 1. Correct Alignment Logic (The "Hard" Part)
 * 2. Thread Safety (Mutex for shared state)
 * 3. Metadata Handling (Storing original pointer)
 * 4. Basic Statistics (To show you care about observability)
 * 
 * Omitted for brevity (but you should mention them verbally):
 * - Guards/Canaries (Buffer overflow protection)
 * - Magic Numbers (Double-free detection)
 * - Detailed per-thread tracking
 ******************************************************************************/

// Global Allocator Lock
// In a real interview, explain: "I'm using a global lock for simplicity, 
// but in production, I'd use per-thread arenas to reduce contention."
static pthread_mutex_t allocator_mutex = PTHREAD_MUTEX_INITIALIZER;

// Basic Statistics
typedef struct {
    size_t current_usage;
} allocator_stats_t;

static allocator_stats_t global_stats = {0};

// Thread-Local Stats (No Lock Needed!)
// Tracks "Net Usage" for this specific thread.
// Positive = Producer (Allocated more than freed)
// Negative = Consumer (Freed more than allocated)
static __thread ssize_t tls_current_usage = 0;

// Metadata: We ONLY store what is strictly necessary to free the memory.
// We need:
// 1. The original pointer (to call free)
// 2. The size (to update stats correctly)
// 3. The thread ID (for basic ownership tracking/debugging)
typedef struct {
    void *original_ptr;
    size_t size;
    pthread_t thread_id;
} metadata_t;

/*******************************************************************************
 * ALIGNED MALLOC
 ******************************************************************************/
void *ts_aligned_malloc(size_t size, size_t alignment) {
    // 1. Validate Alignment (Must be power of 2)
    if ((alignment & (alignment - 1)) != 0 || alignment == 0) {
        return NULL; 
    }

    // Ensure alignment is at least sizeof(metadata_t) so metadata fits in the gap
    if (alignment < sizeof(metadata_t)) {
        alignment = sizeof(metadata_t); 
    }

    // 2. Calculate Total Size (Classic "align.c" Formula)
    // We allocate enough space for:
    // - User Data (size)
    // - Metadata Gap (alignment)
    // - Worst-case Padding (alignment - 1)
    size_t total_size = size + 2 * alignment - 1;

    // 3. Allocate Raw Memory
    // Note: malloc itself is thread-safe, so we don't need to lock here.
    // This is a good optimization to mention.
    void *raw_ptr = malloc(total_size);
    if (!raw_ptr) return NULL;

    // 4. Calculate Aligned Address for User Data
    // We want the User Pointer to be aligned.
    // We also need space for Metadata immediately BEFORE the User Pointer.
    // So we start looking for an aligned address at (raw_ptr + sizeof(metadata_t)).
    uintptr_t raw_addr = (uintptr_t)raw_ptr;
    uintptr_t min_user_addr = raw_addr + sizeof(metadata_t);

    // Round UP to next multiple of 'alignment'
    uintptr_t user_addr = (min_user_addr + alignment - 1) & ~(alignment - 1);
    void *user_ptr = (void *)user_addr;
    
    // 5. Store Metadata
    // We place it immediately before the user pointer.
    // Since we enforced alignment >= sizeof(metadata_t), we know we have space.
    metadata_t *meta = (metadata_t *)(user_addr - sizeof(metadata_t));
    meta->original_ptr = raw_ptr;
    meta->size = size;
    meta->thread_id = pthread_self(); // Track ownership

    // 6. Update Stats
    // A. Thread-Local (Fast, No Lock)
    tls_current_usage += size;

    // B. Global (Requires Lock)
    // We only lock for this brief update. This is "Fine-Grained Locking".
    pthread_mutex_lock(&allocator_mutex);
    global_stats.current_usage += size;
    pthread_mutex_unlock(&allocator_mutex);

    return user_ptr;
}

/*******************************************************************************
 * ALIGNED FREE
 ******************************************************************************/
void ts_aligned_free(void *ptr) {
    if (!ptr) return;

    // 1. Retrieve Metadata
    // We stored it immediately before the user pointer.
    metadata_t *meta = (metadata_t *)((char *)ptr - sizeof(metadata_t));

    // 2. Update Stats
    // A. Thread-Local (Fast, No Lock)
    tls_current_usage -= meta->size;

    // B. Global (Requires Lock)
    pthread_mutex_lock(&allocator_mutex);
    global_stats.current_usage -= meta->size;
    pthread_mutex_unlock(&allocator_mutex);

    // 3. Free the ORIGINAL pointer
    free(meta->original_ptr);
}

/*******************************************************************************
 * TEST HARNESS
 ******************************************************************************/
void* worker(void *arg) {
    int id = *(int*)arg;
    printf("Thread %d starting...\n", id);
    
    for(int i=0; i<1000; i++) {
        void *p = ts_aligned_malloc(1024, 64);
        assert(((uintptr_t)p & 63) == 0); // Verify alignment
        ts_aligned_free(p);
    }
    
    printf("Thread %d done. Net Usage: %zd bytes (Should be 0)\n", id, tls_current_usage);
    return NULL;
}

int main() {
    printf("Starting Bare Bones Thread-Safe Aligned Malloc Test...\n");

    pthread_t t1, t2;
    int id1=1, id2=2;

    pthread_create(&t1, NULL, worker, &id1);
    pthread_create(&t2, NULL, worker, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final Usage (Should be 0): %zu\n", global_stats.current_usage);
    
    return 0;
}
