
#pragma once

#include "tom.h"
#include "stb_ds.h"

enum collision_type {
    RECT,
};

struct collision_tag : tag {
    collision_type CollisionType = RECT;
    union Body {
        Vector2 Size;
    };
    struct { collision_tag *key; collision_tag* value; } *Collisions = nullptr;
};

tag* MakeCollisionTag();
void CollisionTagTick(tag& Tag);
void CollisionTagDraw(const tag& Tag);
bool OnCollisionTagGetMsg(tag& Tag, msg& Msg);