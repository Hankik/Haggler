
#pragma once
#include "allocators.h"
#include "sim.h"


void* BuddyRealloc(void* context, void* ptr, size_t size);
void BuddyFree(void* context, void* ptr);



struct tom_ctx
{
    Buddy_Allocator *BuddyAlloc;
    int IdCounter = 0;
    sim_tag* SimTag;
    struct { int key; obj* value; } *ObjMap = nullptr;
    struct { char * key; Texture2D value; } *TextureMap = nullptr;
    struct { char * key; tray<Texture2D>* value; } *AnimMap = nullptr;
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
