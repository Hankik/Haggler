
#pragma once
#include "allocators.h"
#include "tom.h"

struct tom_ctx
{
    Buddy_Allocator *BuddyAlloc;
    int IdCounter = 0;
    obj* Sim;
};

extern const int ALLOCATOR_SIZE;
extern char *BackingBuffer;
extern Buddy_Allocator *BuddyAlloc;
extern tom_ctx TomCtx;

template <typename T>
void *MakeAlloc()
{
    return BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(T));
}
