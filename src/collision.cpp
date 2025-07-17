#include "collision.h"
#include "context.h"

tag* MakeCollisionTag() {
    collision_tag* CollisionTag = (collision_tag*) MakeAlloc<collision_tag>();
    *CollisionTag = collision_tag{};
    CollisionTag->TickFn = CollisionTagTick;
    CollisionTag->DrawFn = CollisionTagDraw;
    CollisionTag->OnGetMsgFn = OnCollisionTagGetMsg;
    return CollisionTag;
}

void CollisionTagTick(tag& Tag) {
    collision_tag& CollisionTag = (collision_tag&) Tag;

    hmfree(CollisionTag.Collisions);
    
    
}

void CollisionTagDraw(const tag& Tag) {}

bool OnCollisionTagGetMsg(tag& Tag, msg& Msg) {
    return false;
}