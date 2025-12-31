#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h> // The C11 Atomic Library

// ---------------------------------------------------------
// Ring Buffer Structure
// ---------------------------------------------------------
typedef struct {
    uint8_t *buffer;      // The actual data storage
    size_t max_size;      // Total size of the buffer
    
    // ATOMIC INDICES
    // By making these atomic, the compiler ensures that reads/writes
    // are indivisible and synchronized between threads.
    // We add padding to ensure they are on different cache lines (performance).
    atomic_size_t head;   
    uint8_t _padding1[64]; // Prevent False Sharing (Staff-level detail)
    
    atomic_size_t tail;   
    uint8_t _padding2[64]; // Prevent False Sharing
} ring_buffer_t;

// ---------------------------------------------------------
// Initialization
// ---------------------------------------------------------
bool rb_init(ring_buffer_t *rb, size_t size) {
    rb->buffer = malloc(size);
    if (!rb->buffer) return false;

    rb->max_size = size;
    
    // Initialize atomic variables just like normal integers
    atomic_init(&rb->head, 0);
    atomic_init(&rb->tail, 0);
    
    return true;
}

// ---------------------------------------------------------
// Producer: Push Data
// Thread Safety: Only ONE thread should call this function.
// ---------------------------------------------------------
bool rb_push(ring_buffer_t *rb, uint8_t data) {
    // 1. Read indices
    // Reading an atomic variable is safe. We get the current snapshot.
    size_t current_head = rb->head; 
    size_t current_tail = rb->tail;

    // Calculate where the next write would go
    size_t next_head = (current_head + 1) % rb->max_size;

    // 2. Check if full
    // If next_head hits tail, we have wrapped around and hit the reader.
    if (next_head == current_tail) {
        return false; // Buffer is Full
    }

    // 3. Write Data (CRITICAL STEP)
    // We write to the buffer at the current head position.
    // Note: This is NOT atomic. It doesn't need to be!
    // Why? Because the consumer is looking at 'tail', not 'head'.
    // As long as we don't move 'head' yet, the consumer won't look here.
    rb->buffer[current_head] = data;

    // 4. Update Head (ATOMIC COMMIT)
    // This simple assignment is actually doing heavy lifting.
    // The C compiler guarantees that Step 3 (Write Data) is fully complete
    // and visible to other threads BEFORE this line executes.
    // This "publishes" the data to the consumer.
    rb->head = next_head; 
    
    return true;
}

// ---------------------------------------------------------
// Consumer: Pop Data
// Thread Safety: Only ONE thread should call this function.
// ---------------------------------------------------------
bool rb_pop(ring_buffer_t *rb, uint8_t *data) {
    // 1. Read indices
    size_t current_tail = rb->tail;
    size_t current_head = rb->head;

    // 2. Check if empty
    if (current_head == current_tail) {
        return false; // Buffer is Empty
    }

    // 3. Read Data (CRITICAL STEP)
    // We grab the data BEFORE we tell the producer the slot is free.
    *data = rb->buffer[current_tail];

    // 4. Update Tail (ATOMIC COMMIT)
    // This assignment tells the producer: "I am done with this slot."
    // The C compiler guarantees Step 3 (Read Data) finishes BEFORE this happens.
    rb->tail = (current_tail + 1) % rb->max_size;

    return true;
}

// ---------------------------------------------------------
// Cleanup
// ---------------------------------------------------------
void rb_free(ring_buffer_t *rb) {
    free(rb->buffer);
    rb->buffer = NULL;
}