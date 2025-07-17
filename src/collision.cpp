#include "collision.h"
#include "context.h"

tag* MakeCollisionTag() {
    collision_tag* CollisionTag = (collision_tag*) MakeAlloc<collision_tag>();

    *CollisionTag = collision_tag{};
    CollisionTag->TickFn = CollisionTagTick;
    CollisionTag->DrawFn = CollisionTagDraw;
    CollisionTag->OnGetMsgFn = OnCollisionTagGetMsg;
    CollisionTag->CollisionId = TomCtx.SimTag->CollisionIdCounter++;
    CollisionTag->Body.Size = Vector2{32, 32};
    hmput(TomCtx.SimTag->CollisionTagRegistry, CollisionTag->CollisionId, CollisionTag);
    
    return CollisionTag;
}

void CollisionTagTick(tag& Tag) {
    //collision_tag& CollisionTag = (collision_tag&) Tag;

}

void CollisionTagDraw(const tag& Tag) {
    collision_tag& CollisionTag = (collision_tag&) Tag;
    int CollisionCount = hmlen(CollisionTag.Collisions);
    Color DrawColor = WHITE;
    obj* ParentObj = GetObj(Tag);
    if (CollisionCount > 0) {
        DrawColor = RED;
    } else {
    }

    if (CollisionTag.CollisionType == RECT) {

        Vector2 Size = CollisionTag.Body.Size;

        DrawRectangleLinesEx(
            (Rectangle) {
                ParentObj->LocalPos.x - Size.x/2,
                ParentObj->LocalPos.y - Size.y/2,
                CollisionTag.Body.Size.x,
                CollisionTag.Body.Size.y,
            },
            3.0f,
            DrawColor
        );
    }
}

bool OnCollisionTagGetMsg(tag& Tag, msg& Msg) {
    return false;
}

bool IsColliding(const collision_tag& A, const collision_tag& B) {

    obj* ObjA = GetObj(A);
    obj* ObjB = GetObj(B);

    switch (A.CollisionType) {

        case RECT: {

            switch (B.CollisionType) {

                case RECT: {

                    

                    return CheckCollisionRecs(
                        (Rectangle) {
                            ObjA->LocalPos.x - A.Body.Size.x/2,
                            ObjA->LocalPos.y - A.Body.Size.y/2,
                            A.Body.Size.x,
                            A.Body.Size.y,
                        }, 
                        (Rectangle) {
                            ObjB->LocalPos.x - B.Body.Size.x/2,
                            ObjB->LocalPos.y - B.Body.Size.y/2,
                            B.Body.Size.x,
                            B.Body.Size.y,
                        }
                    );
                }
            }
        }
    }

    return false;
}