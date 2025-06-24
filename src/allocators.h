// THIS IS GINGERBILL'S ARENA ALLOCATOR IMPLEMENTATION

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

#pragma once


bool IsPowerOfTwo(uintptr_t x)
{
    return (x & (x - 1)) == 0;
}

uintptr_t AlignForward(uintptr_t ptr, size_t align)
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

void ArenaInit(Arena *a, void *backing_buffer, size_t backing_buffer_length)
{
    a->buf = (unsigned char *)backing_buffer;
    a->buf_len = backing_buffer_length;
    a->curr_offset = 0;
    a->prev_offset = 0;
}

void *ArenaAllocAlign(Arena *a, size_t size, size_t align)
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
void *ArenaAlloc(Arena *a, size_t size)
{
    return ArenaAllocAlign(a, size, DEFAULT_ALIGNMENT);
}

void ArenaFree(Arena *a, void *ptr)
{
    // Do nothing
}

void *ArenaResizeAlign(Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align)
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
void *ArenaResize(Arena *a, void *old_memory, size_t old_size, size_t new_size)
{
    return ArenaResizeAlign(a, old_memory, old_size, new_size, DEFAULT_ALIGNMENT);
}

void ArenaFreeAll(Arena *a)
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

Temp_Arena_Memory temp_arena_memory_begin(Arena *a)
{
    Temp_Arena_Memory temp;
    temp.arena = a;
    temp.prev_offset = a->prev_offset;
    temp.curr_offset = a->curr_offset;
    return temp;
}

void temp_arena_memory_end(Temp_Arena_Memory temp)
{
    temp.arena->prev_offset = temp.prev_offset;
    temp.arena->curr_offset = temp.curr_offset;
}