#include <stdio.h>
#include <stdlib.h>

/* block struct */
struct s_block {
    size_t size;
    struct s_block *next;
    struct s_block *prev;
    int free;
    void *ptr;
    /* A pointer to the allocated block */
    char data[1];
};

typedef struct s_block *t_block;
#define META_SIZE sizeof(struct block_meta)

/* Get the block from and addr */
t_block get_block(void *p)
{
    char *tmp;
    tmp = p;
    return (p = tmp -= BLOCK_SIZE);
}

/* Valid addr for free */
int valid_addr(void *p)
{
    if (base)
    {
        if ( p > base && p < sbrk (0))
        {
            return (p == (get_block(p))->ptr);
        }
    }
    return (0);
}

void *global_base = NULL;

struct block_meta *find_free_block(struct block_meta **last, size_t size) {
    struct block_meta *current = global_base;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

struct block_meta *request_space(struct block_meta* last, size_t size) {
    struct block_meta *block;
    block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    assert((void*)block == request); // Not thread safe.
    if (request == (void*) - 1) {
        return NULL; // sbrk failed.
    }

    if (last) { // NULL on first request.
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->free = 0;
    block->magic = 0x12345678;
    return block;
}

void split_block(t_block b, size_t s) {
    t_block new;
    new = b->data + s;
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->free = 1;
    b->size = s;
    b->next = new;
}

void *malloc(size_t size) {
    t_block b, last;
    size_t s;
    s = align4(size);
    if (base) {
        /* First find a block */
        last = base;
        b = find_block(&last , s);
        if (b) {
            /* can we split */
            if ((b->size - s) >= (BLOCK_SIZE + 4))
                split_block(b, s);
            b->free = 0;
        } else {
            /* No fitting block , extend the heap */
            b = extend_heap(last , s);
            if (!b)
                return (NULL);
        }
    } else {
        /* first time */
        b = extend_heap(NULL , s);
        if (!b)
            return (NULL);
        base = b;
    }
    return (b->data);
}




struct block_meta *get_block_ptr(void *ptr) {
    return (struct block_meta*)ptr - 1;
}


/* The free */
/* See free(3) */
void free(void *p)
{
    t_block b;
    if (valid_addr(p))
    {
        b = get_block(p);
        b->free = 1;
        /* fusion with previous if possible */
        if (b->prev && b->prev->free)
            b = fusion(b->prev);
        /* then fusion with next */
        if (b->next)
            fusion(b);
        else
        {
            /* free the end of the heap */
            if (b->prev)
                b->prev ->next = NULL;
            else
                /* No more block !*/
                base = NULL;
            brk(b);
        }
    }
}

// TODO: consider merging blocks once splitting blocks is implemented.
// struct block_meta* block_ptr = get_block_ptr(ptr);
// assert(block_ptr->free == 0);
// assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
// block_ptr->free = 1;
// block_ptr->magic = 0x55555555;


