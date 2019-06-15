#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t * const buffer;
    int head;
    int tail;
    const int maxlen;
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

    if (next == c->tail)  // if the head + 1 == tail, circular buffer is full
        return -1;

    c->buffer[c->head] = data;  // Load data and then move
    c->head = next;             // head to next data offset.
    return 0;  // return success to indicate successful push.
}

int circ_bbuf_pop(circ_bbuf_t *c, uint8_t *data)
{
    int next;

    if (c->head == c->tail)  // if the head == tail, we don't have any data
        return -1;

    next = c->tail + 1;  // next is where tail will point to after this read.
    if(next >= c->maxlen)
        next = 0;

    *data = c->buffer[c->tail];  // Read data and then move
    c->tail = next;              // tail to next offset.
    return 0;  // return success to indicate successful push.
}


int main() {
	CIRC_BBUF_DEF(my_circ_buf, 32);
    uint8_t out_data=0, in_data = 0x55;

    if (circ_bbuf_push(&my_circ_buf, in_data)) {
        printf("Out of space in CB\n");
        return -1;
    }

    if (circ_bbuf_pop(&my_circ_buf, &out_data)) {
        printf("CB is empty\n");
        return -1;
    }

        if (circ_bbuf_pop(&my_circ_buf, &out_data)) {
        printf("CB is empty\n");
        return -1;
    }

    // here in_data = in_data = 0x55;
    printf("Push: 0x%x\n", in_data);
    printf("Pop:  0x%x\n", out_data);
    return 0;

}