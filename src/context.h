
#pragma once
#include "allocators.h"
#include "sim.h"

#define STBDS_REALLOC(context,ptr,size) BuddyRealloc(context, ptr, size)
#define STBDS_FREE(context,ptr)         BuddyFree(context, ptr)

void* BuddyRealloc(void* context, void* ptr, size_t size);
void BuddyFree(void* context, void* ptr);

struct tom_ctx
{
    Buddy_Allocator *BuddyAlloc;
    int IdCounter = 0;
    sim_tag* SimTag;
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
