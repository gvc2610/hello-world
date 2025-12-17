# Thread-Safe Aligned Memory Allocator

## Staff/Principal Engineer Level Implementation

This is an advanced, production-grade aligned memory allocator with comprehensive debugging and safety features, typical of what would be expected at Staff or Principal Engineer level interviews.

---

## Features Implemented

### ✅ Core Functionality
1. **Thread-Safe Operations** - Uses `pthread_mutex` for all allocations/frees
2. **Arbitrary Alignment** - Supports any power-of-2 alignment
3. **Proper Memory Tracking** - Stores metadata for each allocation

### ✅ Safety & Debugging
4. **Double-Free Detection** - Magic numbers detect freed memory
5. **Use-After-Free Detection** - Poisons freed memory with `0xDD`
6. **Buffer Overflow Detection** - Guard bytes before/after user data
7. **Buffer Underflow Detection** - Front guard validation
8. **Heap Corruption Detection** - Canary values in metadata
9. **Alignment Validation** - Asserts correct alignment at runtime

### ✅ Profiling & Statistics
10. **Memory Leak Detection** - Tracks active allocations
11. **Usage Statistics** - Total/peak memory usage
12. **Thread ID Tracking** - Records which thread allocated
13. **Timestamp Tracking** - When each allocation occurred

---

## Memory Layout

```
┌─────────────────────────────────────────────────────────────────┐
│ malloc() returns here                                            │
├─────────────────────────────────────────────────────────────────┤
│ [METADATA: ~48 bytes]                                           │
│   - magic: 0xDEADBEEF (integrity check)                        │
│   - original_ptr: original malloc address                       │
│   - size: user requested size                                   │
│   - alignment: alignment value                                  │
│   - thread_id: allocating thread                                │
│   - timestamp: allocation time                                  │
│   - canary: 0xCAFEBABE (corruption check)                      │
│   - status: ALLOCATED/FREED                                     │
├─────────────────────────────────────────────────────────────────┤
│ [PADDING: variable, ensures next section aligned]              │
├─────────────────────────────────────────────────────────────────┤
│ [FRONT GUARD: 8 bytes] 0xDEADDEADDEADDEAD                      │
├─────────────────────────────────────────────────────────────────┤
│ [USER DATA: size bytes] ← returned pointer (aligned)           │
├─────────────────────────────────────────────────────────────────┤
│ [REAR GUARD: 8 bytes] 0xBEEFBEEFBEEFBEEF                       │
└─────────────────────────────────────────────────────────────────┘
```

---

## API

### Allocation
```c
void* ts_aligned_malloc(size_t size, size_t alignment);
```
- Returns aligned pointer or NULL on failure
- `alignment` must be power of 2
- Thread-safe

### Deallocation
```c
void ts_aligned_free(void *ptr, size_t alignment);
```
- Must match alignment used in allocation
- Thread-safe
- Aborts on double-free or corruption

### Statistics
```c
void ts_aligned_print_stats(void);
void ts_aligned_set_debug(bool enabled);
```

---

## Example Usage

```c
// Single-threaded
void *ptr = ts_aligned_malloc(1024, 64);  // 64-byte aligned
// ... use ptr ...
ts_aligned_free(ptr, 64);

// Multi-threaded
void* worker(void *arg) {
    void *ptr1 = ts_aligned_malloc(2048, 128);
    void *ptr2 = ts_aligned_malloc(4096, 256);
    
    // Do work...
    
    ts_aligned_free(ptr1, 128);
    ts_aligned_free(ptr2, 256);
    return NULL;
}
```

---

## Error Detection Examples

### Double-Free Detection
```c
void *ptr = ts_aligned_malloc(100, 64);
ts_aligned_free(ptr, 64);
ts_aligned_free(ptr, 64);  // ❌ ABORTS: "DOUBLE-FREE detected!"
```

### Buffer Overflow Detection
```c
void *ptr = ts_aligned_malloc(100, 64);
char *buf = (char *)ptr;
buf[105] = 'X';  // Write past end
ts_aligned_free(ptr, 64);  // ❌ ABORTS: "Buffer overflow detected!"
```

### Use-After-Free Detection
```c
void *ptr = ts_aligned_malloc(100, 64);
ts_aligned_free(ptr, 64);  // Memory poisoned with 0xDD
char *buf = (char *)ptr;
char c = buf[0];  // c == 0xDD (poisoned)
```

---

## Interview Topics Demonstrated

### 1. Systems Programming
- Manual memory management
- Pointer arithmetic
- Binary/bitwise operations
- Memory alignment concepts

### 2. Concurrency
- Mutex-based synchronization
- Thread-safe data structures
- Race condition prevention
- Thread ID tracking

### 3. Defensive Programming
- Input validation
- Error handling
- Assertions
- Graceful degradation

### 4. Debugging Techniques
- Magic numbers
- Canaries
- Guard pages
- Memory poisoning
- Statistics gathering

### 5. Performance Considerations
- Memory overhead analysis
- Lock contention (discussed)
- Cache line alignment
- Allocation strategies

---

## Follow-Up Interview Questions

### Level 1: Understanding
- Q: "Why must alignment be a power of 2?"
- A: The bit-mask formula `& ~(align-1)` only works for powers of 2

### Level 2: Improvements
- Q: "How would you reduce lock contention?"
- A: Per-thread allocation pools, lock-free algorithms, or thread-local caches

### Level 3: Advanced
- Q: "How would you implement `realloc` for aligned memory?"
- A: Need to copy data, preserve alignment, handle metadata carefully

### Level 4: Production
- Q: "How would you integrate this with existing allocators?"
- A: Replace global new/delete, interpose malloc, or use as specialized allocator

---

## Complexity Analysis

| Operation | Time | Space |
|-----------|------|-------|
| Allocation | O(1) | O(size + align + overhead) |
| Free | O(1) with search, O(1) possible | O(1) |
| Stats | O(1) | O(1) |

**Overhead per allocation:** ~60-70 bytes (metadata + guards)

---

## Limitations & Trade-offs

### Current Limitations
1. **Metadata Search** - O(1) but scans memory; could use back-pointer
2. **Lock Granularity** - Single global mutex; could use per-bucket locks
3. **No Coalescing** - Fragmentation possible; could implement buddy allocator
4. **Fixed Overhead** - Always pays for guards; could be conditional

### Design Trade-offs
- **Safety vs Performance** - Chose safety (guards, validation)
- **Simplicity vs Efficiency** - Chose simplicity (metadata search)
- **Memory vs Speed** - Chose memory (detailed statistics)

---

## Production Improvements

For a real production system, consider:

1. **Lock-Free Allocation** - Per-thread pools + CAS operations
2. **Size Classes** - Separate allocators for different sizes
3. **NUMA Awareness** - Allocate from local memory node
4. **Compaction** - Defragmentation for long-running processes
5. **Integration** - Hooks into system allocator or custom malloc
6. **Telemetry** - Export metrics to monitoring systems
7. **Configuration** - Runtime tuning of guard sizes, debug levels

---

## Testing

Run the included test suite:
```bash
gcc -o thread_safe_aligned_malloc thread_safe_aligned_malloc.c -lpthread
./thread_safe_aligned_malloc
```

Expected output:
- ✅ All allocations properly aligned
- ✅ Multi-threaded test completes successfully
- ✅ No memory leaks detected
- ✅ Statistics show all allocations freed

---

## Conclusion

This implementation demonstrates:
- **Deep systems knowledge** - Memory management, alignment, concurrency
- **Production mindset** - Error handling, logging, statistics
- **Defensive coding** - Multiple layers of validation
- **Performance awareness** - Understanding trade-offs

**Typical for:** Staff Engineer, Principal Engineer, or Senior Systems roles requiring expertise in low-level programming and memory management.
