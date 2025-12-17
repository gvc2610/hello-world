#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>  // for uintptr_t type, needed for pointer arithmetic
 
/*******************************************************************************
 * ALIGNED MALLOC IMPLEMENTATION
 * 
 * Purpose: Allocate memory aligned to a specified byte boundary (e.g., 64 bytes)
 *          and store metadata to enable proper deallocation.
 * 
 * MEMORY LAYOUT:
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ malloc() returns here (could be anywhere, typically 16-byte aligned)    │
 * │    ↓                                                                     │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ [PADDING: 0 to (align-1) bytes]                                         │
 * │  - Skipped bytes to reach next align-byte boundary                      │
 * │  - Size varies depending on malloc's returned address                   │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ [METADATA: align bytes reserved, 8 bytes used on 64-bit]               │
 * │  - Starts at first align-byte boundary                                  │
 * │  - Stores original malloc pointer (uintptr_t = 8 bytes)                │
 * │  - Remaining (align - 8) bytes unused but ensure next alignment         │
 * ├─────────────────────────────────────────────────────────────────────────┤
 * │ [USER DATA: size bytes]  ← This pointer returned to user               │
 * │  - Guaranteed to be align-byte aligned                                  │
 * │  - User can safely perform aligned memory operations                    │
 * └─────────────────────────────────────────────────────────────────────────┘
 * 
 * ALLOCATION FORMULA: size + 2*align - 1
 *   - size:       User's requested bytes
 *   - align:      Bytes reserved for metadata (ensures next pointer aligned)
 *   - align - 1:  Maximum padding needed (worst case: malloc returns addr+1)
 * 
 * ALIGNMENT FORMULA: (ptr + align - 1) & ~(align - 1)
 *   - Rounds pointer UP to next align-byte boundary
 *   - Works because align must be power of 2 (e.g., 64 = 2^6)
 *   - Example for 64-byte: ~63 = ...11000000, masks out lower 6 bits
 * 
 * FREE PROCESS:
 *   1. User calls freealigned(aligned_ptr, align)
 *   2. Go back 'align' bytes to reach metadata location
 *   3. Read stored original malloc pointer
 *   4. Call free() on original pointer
 *
 ******************************************************************************/
void * mallocaligned(size_t size, int align) {
    // Ensure minimum alignment for storing a pointer
    if (align < sizeof(int))
        align = sizeof(int);
    
    // Allocate: size + metadata_space + worst_case_padding
    //         = size + align + (align - 1)
    //         = size + 2*align - 1
    void *ptr1 = malloc(size + align + align - 1);
    printf("%zu bytes of memory allocated at %p\n", size+2*align-1, ptr1);
    
    // Round UP to next align-byte boundary using bit masking
    // Formula: (ptr + align - 1) & ~(align - 1)
    // - Adding (align-1) ensures we reach next boundary if not aligned
    // - AND with ~(align-1) zeros out lower bits, giving aligned address
    void *ptr2 = (void *)(((uintptr_t)ptr1 + align - 1) & ~(align-1));
    
    // Store original malloc pointer at this aligned location
    // This is our metadata - needed later to free the memory
    *(uintptr_t *)ptr2 = (uintptr_t)ptr1;
    
    // Advance by 'align' bytes to give user their aligned memory
    // Mathematical property: aligned_address + align = aligned_address
    // This ensures user pointer is also aligned to 'align' bytes
    ptr2 = ptr2 + align;
    printf("aligned memory at %p\n", ptr2);
 
    return ptr2;
}
 
/*******************************************************************************
 * FREE ALIGNED MEMORY
 * 
 * Reverses the allocation process to retrieve and free original malloc pointer
 * 
 * MEMORY TRAVERSAL:
 *        User has this pointer (aligned)
 *                   ↓
 *   ┌───────────────────────────────┐
 *   │ [USER DATA]                   │ ← ptr parameter
 *   ├───────────────────────────────┤
 *   │ Go back 'align' bytes         │ ← ptr - align
 *   ├───────────────────────────────┤
 *   │ [METADATA: original ptr]      │ ← Read uintptr_t value here
 *   ├───────────────────────────────┤
 *   │ [PADDING]                     │
 *   ├───────────────────────────────┤
 *   │ Original malloc address       │ ← free() this address
 *   └───────────────────────────────┘
 *
 ******************************************************************************/
void freealigned(void *ptr, int align) {
    // Step back 'align' bytes to where we stored the metadata
    ptr = (void *)((uintptr_t)ptr - align);
    
    // Retrieve the original malloc pointer stored at this location
    ptr = (void *)(*(uintptr_t *)ptr);
    printf("free memory at address %p\n", ptr);
    
    // Free the original malloc'd memory
    free(ptr);
}
 
int main() {
    // Example: Allocate 1000 bytes aligned to 64-byte boundary
    // Common use case: SIMD operations, cache line alignment, DMA transfers
    int align = 64;  // Must match in both malloc and free!
    void *ptr = mallocaligned(1000, align);
    printf("allocated pointer at %p\n", ptr);
    
    // User can now safely use ptr for aligned memory operations
    // ... do work with ptr ...
    
    // Free using the same alignment value
    freealigned(ptr, align);
    return 0;
}