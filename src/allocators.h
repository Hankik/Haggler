// THIS IS GINGERBILL'S ARENA AND BUDDY ALLOCATOR IMPLEMENTATIONS

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include <stddef.h>
#include <stdint.h>

#if !defined(__cplusplus)
#if (defined(_MSC_VER) && _MSC_VER < 1800) || (!defined(_MSC_VER) && !defined(__STDC_VERSION__))
#ifndef true
#define true (0 == 0)
#endif
#ifndef false
#define false (0 != 0)
#endif
typedef unsigned char bool;
#else
#include <stdbool.h>
#endif
#endif

#include <stdio.h>
#include <assert.h>
#include <string.h>



static inline bool IsPowerOfTwo(uintptr_t x)
{
    return (x & (x - 1)) == 0;
}

static inline uintptr_t AlignForward(uintptr_t ptr, size_t align)
{
    uintptr_t p, a, modulo;

    assert(IsPowerOfTwo(align));

    p = ptr;
    a = (uintptr_t)align;
    // Same as (p % a) but faster as 'a' is a power of two
    modulo = p & (a - 1);

    if (modulo != 0)
    {
        // If 'p' address is not aligned, push the address to the
        // next value which is aligned
        p += a - modulo;
    }
    return p;
}

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

typedef struct Arena Arena;
struct Arena
{
    unsigned char *buf;
    size_t buf_len;
    size_t prev_offset; // This will be useful for later on
    size_t curr_offset;
};

static inline void ArenaInit(Arena *a, void *backing_buffer, size_t backing_buffer_length)
{
    a->buf = (unsigned char *)backing_buffer;
    a->buf_len = backing_buffer_length;
    a->curr_offset = 0;
    a->prev_offset = 0;
}

static inline void *ArenaAllocAlign(Arena *a, size_t size, size_t align)
{
    // Align 'curr_offset' forward to the specified alignment
    uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
    uintptr_t offset = AlignForward(curr_ptr, align);
    offset -= (uintptr_t)a->buf; // Change to relative offset

    // Check to see if the backing memory has space left
    if (offset + size <= a->buf_len)
    {
        void *ptr = &a->buf[offset];
        a->prev_offset = offset;
        a->curr_offset = offset + size;

        // Zero new memory by default
        memset(ptr, 0, size);
        return ptr;
    }
    // Return NULL if the arena is out of memory (or handle differently)
    return NULL;
}

// Because C doesn't have default parameters
static inline void *ArenaAlloc(Arena *a, size_t size)
{
    return ArenaAllocAlign(a, size, DEFAULT_ALIGNMENT);
}

static inline void ArenaFree(Arena *a, void *ptr)
{
    // Do nothing
}

static inline void *ArenaResizeAlign(Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align)
{
    unsigned char *old_mem = (unsigned char *)old_memory;

    assert(IsPowerOfTwo(align));

    if (old_mem == NULL || old_size == 0)
    {
        return ArenaAllocAlign(a, new_size, align);
    }
    else if (a->buf <= old_mem && old_mem < a->buf + a->buf_len)
    {
        if (a->buf + a->prev_offset == old_mem)
        {
            a->curr_offset = a->prev_offset + new_size;
            if (new_size > old_size)
            {
                // Zero the new memory by default
                memset(&a->buf[a->curr_offset], 0, new_size - old_size);
            }
            return old_memory;
        }
        else
        {
            void *new_memory = ArenaAllocAlign(a, new_size, align);
            size_t copy_size = old_size < new_size ? old_size : new_size;
            // Copy across old memory to the new memory
            memmove(new_memory, old_memory, copy_size);
            return new_memory;
        }
    }
    else
    {
        assert(0 && "Memory is out of bounds of the buffer in this arena");
        return NULL;
    }
}

// Because C doesn't have default parameters
static inline void *ArenaResize(Arena *a, void *old_memory, size_t old_size, size_t new_size)
{
    return ArenaResizeAlign(a, old_memory, old_size, new_size, DEFAULT_ALIGNMENT);
}

static inline void ArenaFreeAll(Arena *a)
{
    a->curr_offset = 0;
    a->prev_offset = 0;
}

// Extra Features
typedef struct Temp_Arena_Memory Temp_Arena_Memory;
struct Temp_Arena_Memory
{
    Arena *arena;
    size_t prev_offset;
    size_t curr_offset;
};

static inline Temp_Arena_Memory temp_arena_memory_begin(Arena *a)
{
    Temp_Arena_Memory temp;
    temp.arena = a;
    temp.prev_offset = a->prev_offset;
    temp.curr_offset = a->curr_offset;
    return temp;
}

static inline void temp_arena_memory_end(Temp_Arena_Memory temp)
{
    temp.arena->prev_offset = temp.prev_offset;
    temp.arena->curr_offset = temp.curr_offset;
}


typedef struct Buddy_Block Buddy_Block;
struct Buddy_Block { // Allocation header (metadata)
    size_t size; 
    bool   is_free;
};

static inline Buddy_Block *buddy_block_next(Buddy_Block *block) {
    return (Buddy_Block *)((char *)block + block->size);
}

static inline Buddy_Block *buddy_block_split(Buddy_Block *block, size_t size) {
    if (block != NULL && size != 0) {
        // Recursive split
        while (size < block->size) {
            size_t sz = block->size >> 1;
            block->size = sz;
            block = buddy_block_next(block);
            block->size = sz;
            block->is_free = true;
        }
        
        if (size <= block->size) {
            return block;
        }
    }
    
    // Block cannot fit the requested allocation size
    return NULL;
}

static inline Buddy_Block *buddy_block_find_best(Buddy_Block *head, Buddy_Block *tail, size_t size) {
    // Assumes size != 0
    
    Buddy_Block *best_block = NULL;
    Buddy_Block *block = head;                    // Left Buddy
    Buddy_Block *buddy = buddy_block_next(block); // Right Buddy
     
    // The entire memory section between head and tail is free, 
    // just call 'buddy_block_split' to get the allocation
    if (buddy == tail && block->is_free) {
        return buddy_block_split(block, size);
    }
    
    // Find the block which is the 'best_block' to requested allocation sized
    while (block < tail && buddy < tail) { // make sure the buddies are within the range
        // If both buddies are free, coalesce them together
        // NOTE: this is an optimization to reduce fragmentation
        //       this could be completely ignored
        if (block->is_free && buddy->is_free && block->size == buddy->size) {
            block->size <<= 1;
            if (size <= block->size && (best_block == NULL || block->size <= best_block->size)) {
                best_block = block;
            }
            
            block = buddy_block_next(buddy);
            if (block < tail) {
                // Delay the buddy block for the next iteration
                buddy = buddy_block_next(block);
            }
            continue;
        }
        
                
        if (block->is_free && size <= block->size && 
            (best_block == NULL || block->size <= best_block->size)) {
            best_block = block;
        }
        
        if (buddy->is_free && size <= buddy->size &&
            (best_block == NULL || buddy->size < best_block->size)) { 
            // If each buddy are the same size, then it makes more sense 
            // to pick the buddy as it "bounces around" less
            best_block = buddy;
        }
        
        if (block->size <= buddy->size) {
            block = buddy_block_next(buddy);
            if (block < tail) {
                // Delay the buddy block for the next iteration
                buddy = buddy_block_next(block);
            }
        } else {
            // Buddy was split into smaller blocks
            block = buddy;
            buddy = buddy_block_next(buddy);
        }
    }
    
    if (best_block != NULL) {
        // This will handle the case if the 'best_block' is also the perfect fit
        return buddy_block_split(best_block, size);
    }
    
    // Maybe out of memory
    return NULL;
}

typedef struct Buddy_Allocator Buddy_Allocator;
struct Buddy_Allocator {
    Buddy_Block *head; // same pointer as the backing memory data
    Buddy_Block *tail; // sentinel pointer representing the memory boundary
    size_t alignment; 
};

static inline void BuddyAllocatorInit(Buddy_Allocator *b, void *data, size_t size, size_t alignment) {
    assert(data != NULL);
    assert(IsPowerOfTwo(size) && "size is not a power-of-two");
    assert(IsPowerOfTwo(alignment) && "alignment is not a power-of-two");
    
    // The minimum alignment depends on the size of the `Buddy_Block` header
    assert(IsPowerOfTwo(sizeof(Buddy_Block)));// == 0);
    if (alignment < sizeof(Buddy_Block)) {
        alignment = sizeof(Buddy_Block);
    }
    assert((uintptr_t)data % alignment == 0 && "data is not aligned to minimum alignment");
    
    b->head          = (Buddy_Block *)data;
    b->head->size    = size;
    b->head->is_free = true;
    
    // The tail here is a sentinel value and not a true block
    b->tail = buddy_block_next(b->head);
    
    b->alignment = alignment;
}

static inline size_t buddy_block_size_required(Buddy_Allocator *b, size_t size) {
    size_t actual_size = b->alignment;
    
    size += sizeof(Buddy_Block);
    size = AlignForward(size, b->alignment); 
    
    while (size > actual_size) {
        actual_size <<= 1;
    }
    
    return actual_size;
}

static inline void BuddyBlockCoallescence(Buddy_Block *head, Buddy_Block *tail) {
    for (;;) { 
        // Keep looping until there are no more buddies to coalesce
        
        Buddy_Block *block = head;   
        Buddy_Block *buddy = buddy_block_next(block);   
        
        bool no_coalescence = true;
        while (block < tail && buddy < tail) { // make sure the buddies are within the range
            if (block->is_free && buddy->is_free && block->size == buddy->size) {
                // Coalesce buddies into one
                block->size <<= 1;
                block = buddy_block_next(block);
                if (block < tail) {
                    buddy = buddy_block_next(block);
                    no_coalescence = false;
                }
            } else if (block->size < buddy->size) {
                // The buddy block is split into smaller blocks
                block = buddy;
                buddy = buddy_block_next(buddy);
            } else {
                block = buddy_block_next(buddy);
                if (block < tail) {
                    // Leave the buddy block for the next iteration
                    buddy = buddy_block_next(block);
                }
            }
        }
        
        if (no_coalescence) {
            return;
        }
    }
}

static inline void *BuddyAllocatorAlloc(Buddy_Allocator *b, size_t size) {
    if (size != 0) {    
        size_t actual_size = buddy_block_size_required(b, size);
        
        Buddy_Block *found = buddy_block_find_best(b->head, b->tail, actual_size);
        if (found == NULL) {
            // Try to coalesce all the free buddy blocks and then search again
            BuddyBlockCoallescence(b->head, b->tail);
            found = buddy_block_find_best(b->head, b->tail, actual_size);
        }
            
        if (found != NULL) {
            found->is_free = false;
            return (void *)((char *)found + b->alignment);
        }
        
        // Out of memory (possibly due to too much internal fragmentation)
    }
    
    return NULL;
}

static inline void BuddyAllocatorFree(Buddy_Allocator *b, void *data) {
    if (data != NULL) {
        Buddy_Block *block;
        
        assert(b->head <= data);
        assert(data < b->tail);
        
        block = (Buddy_Block *)((char *)data - b->alignment);
        block->is_free = true;
        
        // NOTE: Coalescence could be done now but it is optional
        // buddy_block_coalescence(b->head, b->tail);
    }
}

#endif 