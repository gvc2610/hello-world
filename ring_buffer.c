#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Theory of Operation:
 *
 * This Ring Buffer (Circular Buffer) implementation uses a "One Slot Open" strategy
 * to distinguish between the "Buffer Full" and "Buffer Empty" states using only
 * the 'head' and 'tail' indices.
 *
 * - buffer: Pointer to the allocated memory for data.
 * - head: Index where the NEXT byte will be written (Producer index).
 * - tail: Index where the NEXT byte will be read (Consumer index).
 * - maxlen: Total size of the allocated buffer.
 *
 * State Definitions:
 * - Empty: head == tail
 * - Full:  (head + 1) % maxlen == tail
 *
 * Because of the "Full" definition, the usable capacity of the buffer is
 * (maxlen - 1) bytes. One byte is always left unused to prevent the 'head'
 * from catching up to the 'tail' completely, which would make it look 'Empty'.
 *
 * Wrap-around is handled by checking if the next index equals 'maxlen' and
 * resetting it to 0.
 */

typedef struct {
    uint8_t * const buffer;
    int head;           // Producer index: points to the next free slot
    int tail;           // Consumer index: points to the next valid data
    const int maxlen;   // Total size of the buffer array
} circ_bbuf_t;

#define CIRC_BBUF_DEF(x,y)                \
    uint8_t x##_data_space[y];            \
    circ_bbuf_t x = {                     \
        .buffer = x##_data_space,         \
        .head = 0,                        \
        .tail = 0,                        \
        .maxlen = y                       \
    }

int circ_bbuf_push(circ_bbuf_t *c, uint8_t data)
{
    int next;

    next = c->head + 1;  // next is where head will point to after this write.
    if (next >= c->maxlen)
        next = 0;

    // Check if the buffer is full.
    // If the next head position equals the current tail, we cannot write
    // because we must leave one slot open to distinguish full from empty.
    if (next == c->tail)
        return -1;

    c->buffer[c->head] = data;  // Load data at current head
    c->head = next;             // Update head to the next position
    return 0;  // return success
}

int circ_bbuf_pop(circ_bbuf_t *c, uint8_t *data)
{
    int next;

    // Check if the buffer is empty.
    // If head equals tail, there is no data to read.
    if (c->head == c->tail)
        return -1;

    next = c->tail + 1;  // next is where tail will point to after this read.
    if(next >= c->maxlen)
        next = 0;

    *data = c->buffer[c->tail];  // Read data from current tail
    c->tail = next;              // Update tail to the next position
    return 0;  // return success
}


int main() {
    // Define a small buffer for testing corner cases.
    // Size 4 means capacity is 3 (one slot open).
    CIRC_BBUF_DEF(my_circ_buf, 4);
    uint8_t out_data = 0;
    int i;

    printf("Buffer Size: %d, Usable Capacity: %d\n", my_circ_buf.maxlen, my_circ_buf.maxlen - 1);

    // 1. Test Empty Pop
    printf("\n--- Test 1: Pop from Empty ---\n");
    if (circ_bbuf_pop(&my_circ_buf, &out_data)) {
        printf("PASS: CB is empty as expected.\n");
    } else {
        printf("FAIL: Pop succeeded on empty buffer.\n");
    }

    // 2. Test Fill to Capacity
    printf("\n--- Test 2: Fill to Capacity ---\n");
    for (i = 0; i < 3; i++) {
        if (circ_bbuf_push(&my_circ_buf, (uint8_t)(i + 0x50)) == 0) {
            printf("Push 0x%x: OK (head=%d, tail=%d)\n", i + 0x50, my_circ_buf.head, my_circ_buf.tail);
        } else {
            printf("Push 0x%x: FAILED (head=%d, tail=%d)\n", i + 0x50, my_circ_buf.head, my_circ_buf.tail);
        }
    }

    // 3. Test Push to Full Buffer
    printf("\n--- Test 3: Push to Full Buffer ---\n");
    if (circ_bbuf_push(&my_circ_buf, 0x99)) {
        printf("PASS: Out of space as expected (head=%d, tail=%d).\n", my_circ_buf.head, my_circ_buf.tail);
    } else {
        printf("FAIL: Push succeeded on full buffer.\n");
    }

    // 4. Test Pop All
    printf("\n--- Test 4: Empty the Buffer ---\n");
    while (circ_bbuf_pop(&my_circ_buf, &out_data) == 0) {
        printf("Pop: 0x%x (head=%d, tail=%d)\n", out_data, my_circ_buf.head, my_circ_buf.tail);
    }

    // 5. Test Wrap Around
    printf("\n--- Test 5: Wrap Around ---\n");
    // Push 2 items
    circ_bbuf_push(&my_circ_buf, 0xA1);
    circ_bbuf_push(&my_circ_buf, 0xA2);
    printf("Pushed 2 items. (head=%d, tail=%d)\n", my_circ_buf.head, my_circ_buf.tail);
    
    // Pop 1 item
    circ_bbuf_pop(&my_circ_buf, &out_data);
    printf("Popped 1 item: 0x%x. (head=%d, tail=%d)\n", out_data, my_circ_buf.head, my_circ_buf.tail);

    // Push 2 more items (should wrap head)
    circ_bbuf_push(&my_circ_buf, 0xA3);
    circ_bbuf_push(&my_circ_buf, 0xA4);
    printf("Pushed 2 more. (head=%d, tail=%d)\n", my_circ_buf.head, my_circ_buf.tail);

    return 0;
}