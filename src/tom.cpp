#include "raylib.h"
#include "tom.h"
#include "context.h"
#include "tray.h"

obj* MakeObj() {
    obj* NewObj = (obj*) BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(obj));
    if (NewObj) {
        NewObj->Id = TomCtx.IdCounter++;
        return NewObj;
    }
    return nullptr;
}

obj* MakeSimObj() {
    obj* Sim = MakeObj();
    if (Sim) {
        Sim->Children = MakeTray<obj*>(64);
        Sim->Tags = MakeTray<tag*>(8);
        return Sim;
    }
    return nullptr;
}

tag* MakeTag(tag_type Type) {
    switch (Type) {
        case EMPTY: {
            tag* NewTag = (tag*) BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(tag)); 
            if (NewTag) {
                NewTag->TickFn = &TagTick;
                NewTag->DrawFn = &TagDraw;
                NewTag->OnGetMsgFn = &OnGetMsg;
                return NewTag;
            }
        } break;
    }
    return nullptr;
}


void ObjTick(obj* Obj)
{

}

void ObjDraw(const obj& Obj)
{

}

void TagTick(tag* Tag) {

}

void TagDraw(const tag& Tag) {

}

bool TryAddObjs(obj* Obj, const tray<obj*>& NewObjs) {
    if (Obj->Children->Amt + NewObjs.Amt <= Obj->Children->Cap) {
        for (int Index = 0; Index < NewObjs.Amt; ++Index) {
            TrayAdd(Obj->Children, NewObjs[Index]);
        }
        return true;
    } else {
        return false;
    }
}

bool TryAddTags(obj* Obj, const tray<tag*>& NewTags) {
    if (Obj->Tags->Amt + NewTags.Amt <= Obj->Tags->Cap) {
        for (int Index = 0; Index < NewTags.Amt; ++Index) {
            TrayAdd(Obj->Tags, NewTags[Index]);
        }
        return true;
    } else {
        return false;
    }
    
}

bool MsgUp(const obj& Obj, const msg& Msg) {
    tray<tag*> Tags = *Obj.Tags;
    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        bool Consumed = OnGetMsg(Tags[Index], Msg);
        if (Consumed) {
            return true;
        }
    }
    if (Obj.Parent) {
        bool Consumed = MsgUp(*Obj.Parent, Msg);
        if (Consumed) {
            return true;
        }
    }
    return false;
}

bool MsgDown(const obj& Obj, const msg& Msg) {
    tray<obj*> Children = *Obj.Children;
    for (int Index = 0; Index < Obj.Children->Amt; ++Index) {
        bool Consumed = MsgDown(*Children[Index], Msg);
        if (Consumed) {
            return true;
        }
    }
    tray<tag*> Tags = *Obj.Tags;
    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        bool Consumed = OnGetMsg(Tags[Index], Msg);
        if (Consumed) {
            return true;
        }
    }
    return false;
}

bool MsgTo(const obj& Obj, const msg& Msg) {
    tray<tag*> Tags = *Obj.Tags;
    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        bool Consumed = OnGetMsg(Tags[Index], Msg);
        if (Consumed) {
            return true;
        }
    }
    return false;
}

bool OnGetMsg(tag* tag, const msg& Msg) {
    return false;
}