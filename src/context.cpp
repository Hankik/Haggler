#include "context.h"
#include <malloc.h>
#include "tray.h"

const int ALLOCATOR_SIZE = 1048576;
char * BackingBuffer = (char*) _aligned_malloc(ALLOCATOR_SIZE, 16);
Buddy_Allocator* BuddyAlloc = (Buddy_Allocator*) _aligned_malloc(sizeof(Buddy_Allocator), 16);


tom_ctx TomCtx {
    BuddyAlloc, 
    0,
    nullptr,
};

void* BuddyRealloc(void* Context, void* Ptr, size_t Size) {
    if (Ptr == NULL) {
        // Allocate new memory
        return BuddyAllocatorAlloc(TomCtx.BuddyAlloc, Size);
    } else if (Size == 0) {
        // Free memory
        BuddyAllocatorFree(TomCtx.BuddyAlloc, Ptr);
        return NULL;
    } else {
        // No true realloc, so allocate new, copy, free old
        void* new_ptr = BuddyAllocatorAlloc(TomCtx.BuddyAlloc, Size);
        if (new_ptr && Ptr) {
            // You need to know the old size; if not tracked, you may have to over-copy
            memcpy(new_ptr, Ptr, Size); // WARNING: may over-copy!
            BuddyAllocatorFree(TomCtx.BuddyAlloc, Ptr);
        }
        return new_ptr;
    }
}

void BuddyFree(void* Context, void* ptr) {
    BuddyAllocatorFree(TomCtx.BuddyAlloc, ptr);
}