#include <stdio.h>
#include <stdlib.h>

/*
 * Theory of Operation: Aligned Memory Allocation
 *
 * Memory Layout Visualization:
 *
 *                 [ Alloc. ]                                          [ sizeof ]
 *                 [ by     ]                                          [ void*  ]   |<---- size ---->|
 *                 [ malloc ]                                          [        ]   |                |
 *      p1 ------> +---------------------------------------------------+--------+--------------------+
 *                 |                                                   |        |                    |
 *                 |                  Unused Padding                   | STORED |     User Data      |
 *                 |                                                   |   P1   |     (Aligned)      |
 *                 |                                                   |        |                    |
 *                 +---------------------------------------------------+--------+--------------------+
 *                 ^                                                   ^        ^
 *                 |                                                   |        |
 *    Start of block returned                                          |        p2 (Returned to user)
 *    by malloc (p1)                                                   |        (Multiple of alignment)
 *                                                                     |
 *                                                             Address where p1 is saved
 *                                                             (p2 - sizeof(void*))
 *
 * Logic:
 * 1. Allocate extra space: 'alignment' bytes (for shifting) + 'sizeof(void*)' (for storing p1).
 * 2. Calculate aligned address (p2): Start from p1 + sizeof(void*), then round up to next alignment.
 * 3. Store the "receipt" (p1) immediately before p2 so aligned_free can find it.
 */

void* aligned_malloc(size_t size, size_t alignment) {
    // 1. Allocate enough space:
    //    User Size + Alignment (for shifting) + sizeof(void*) (for the receipt)
    void* p1 = malloc(size + alignment + sizeof(void*));
    
    if (!p1) return NULL; // Always check for NULL!

    // 2. Calculate the Aligned Address
    //    Start at p1 + sizeof(void*) (make room for the receipt first)
    size_t addr = (size_t)p1 + sizeof(void*);
    
    //    Round up to the next multiple of 'alignment'
    //    Math: (addr + align - 1) & ~(align - 1)
    void* p2 = (void*)((addr + alignment - 1) & ~(alignment - 1));

    // 3. Store the Receipt
    //    Go back 1 pointer size from the aligned address
    //    Cast to void** so we can write a pointer value
    *((void**)((size_t)p2 - sizeof(void*))) = p1;

    // 4. Return the Aligned Pointer
    return p2;
}

void aligned_free(void* p2) {
    // 1. Retrieve the Receipt
    //    Go back 1 pointer size from the input address
    void* p1 = *((void**)((size_t)p2 - sizeof(void*)));

    // 2. Free the original pointer (p1)
    free(p1);
}