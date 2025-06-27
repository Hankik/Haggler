#include "context.h"
#include <malloc.h>

const int ALLOCATOR_SIZE = 1048576;
char * BackingBuffer = (char*) _aligned_malloc(ALLOCATOR_SIZE, 16);
Buddy_Allocator* BuddyAlloc = (Buddy_Allocator*) _aligned_malloc(sizeof(Buddy_Allocator), 16);
tom_ctx TomCtx {BuddyAlloc, 0};