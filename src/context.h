
#include "allocators.h"

#pragma once

struct tom_ctx {
    Buddy_Allocator* BuddyAlloc;
    int IdCounter = 0;
};

namespace {
    const int ALLOCATOR_SIZE = 1048576;
    char * BackingBuffer = (char*) _aligned_malloc(sizeof(char) * ALLOCATOR_SIZE, 16);
    Buddy_Allocator BuddyAlloc;
    tom_ctx TomCtx { &BuddyAlloc, 0 };
}

