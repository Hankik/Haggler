
#pragma once

#include "tom.h"
#include "stb_ds.h"

enum collision_type {
    RECT,
};

struct collision_tag : tag {
    int CollisionId = -1;
    collision_type CollisionType = RECT;
    union collision_body {
        Vector2 Size;
    } Body;
    struct { int key; collision_tag* value; } *Collisions = nullptr;
};

tag* MakeCollisionTag();
void CollisionTagTick(tag& Tag);
void CollisionTagDraw(const tag& Tag);
bool OnCollisionTagGetMsg(tag& Tag, msg& Msg);
bool IsColliding(const collision_tag& A, const collision_tag& B);