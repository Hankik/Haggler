#include "raylib.h"
#include "tom.h"
#include "context.h"
#include "tray.h"
#include "sim.h"
#include "player.h"
#include "button.h"
#include "stb_ds.h"

obj* MakeObj() {
    obj* NewObj = (obj*) BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(obj));
    if (NewObj) {
        *NewObj = obj{};
        NewObj->Id = TomCtx.IdCounter++;
        hmput(TomCtx.ObjMap, NewObj->Id, NewObj);
        return NewObj;
    }
    return nullptr;
}

tag* GetObjTag(obj& Obj, tag_type Type) {

    return hmget(Obj.Tags, Type);
}

void ObjTick(obj& Obj)
{
    for (int Index = 0; Index < Obj.Children->Amt; ++Index) {
        obj* Child = (*Obj.Children)[Index];
        if (Child && Child->State == obj_state::ALIVE) {
            ObjTick(*Child);
        }
    }
    int TagCount = hmlen(Obj.Tags);
    for (int Index = 0; Index < TagCount; ++Index) {
        tag* Tag = Obj.Tags[Index].value;
        if (Tag) {
            (*Tag).TickFn(*Tag);
        }
    }
    
}

Vector2 GetGlobalPos(const obj& Obj) {
    Vector2 PosSum = Obj.LocalPos;
    //obj* CurrentParent = Obj.Parent;
    obj* CurrentParent = hmget(TomCtx.ObjMap, Obj.ParentId);
    while (CurrentParent) {
        PosSum = Vector2Add(PosSum, CurrentParent->LocalPos);
        CurrentParent = hmget(TomCtx.ObjMap, CurrentParent->ParentId);
    }
    return PosSum;
}

void ObjDraw(const obj& Obj)
{
    for (int Index = 0; Index < Obj.Children->Amt; ++Index) {
        obj* Child = (*Obj.Children)[Index];
        if (Child && Child->Visible) {
            ObjDraw(*Child);
        }
    }
    int TagCount = hmlen(Obj.Tags);
    for (int Index = 0; Index < TagCount; ++Index) {
        tag *Tag = Obj.Tags[Index].value;
        if (Tag && Tag->Visible)
        { 
            (*Tag).DrawFn(*Tag);
        }
    }
    
}

void TagTick(tag& Tag) {

}

void TagDraw(const tag& Tag) {

}

bool TryAddObjs(obj& Obj, const tray<obj*>& NewObjs) {
    if (Obj.Children->Amt + NewObjs.Amt <= Obj.Children->Cap) {
        for (int Index = 0; Index < NewObjs.Amt; ++Index) {
            obj* ObjToAdd = NewObjs[Index];
            ObjToAdd->ParentId = Obj.Id;
            TrayAdd(Obj.Children, ObjToAdd);
        }
        return true;
    } else {
        return false;
    }
}

void AddTags(obj& Obj, const tray<tag*>& NewTags) {
    for (int Index = 0; Index < NewTags.Amt; ++Index) {
        tag* TagToAdd = NewTags[Index];
        TagToAdd->ObjId = Obj.Id;
        hmput(Obj.Tags, TagToAdd->Type, TagToAdd);
    }
}

bool MsgUp(obj& Obj, msg& Msg) {
    int TagCount = hmlen(Obj.Tags);
    for (int Index = 0; Index < TagCount; ++Index) {
        tag& Tag = *Obj.Tags[Index].value;
        bool Consumed = Tag.OnGetMsgFn(Tag, Msg);
        if (Consumed) {
            return true;
        }
    }
    if (Obj.ParentId > -1) {
        bool Consumed = MsgUp(*hmget(TomCtx.ObjMap, Obj.ParentId), Msg);
        if (Consumed) {
            return true;
        }
    }
    return false;
}

bool MsgDown(obj& Obj, msg& Msg) {
    int TagCount = hmlen(Obj.Tags);
    for (int Index = 0; Index < TagCount; ++Index) {
        tag& Tag = *Obj.Tags[Index].value;
        bool Consumed = Tag.OnGetMsgFn(Tag, Msg);
        if (Consumed) {
            return true;
        }
    }
    tray<obj*> Children = *Obj.Children;
    for (int Index = 0; Index < Obj.Children->Amt; ++Index) {
        obj& Child = *Children[Index];
        bool Consumed = MsgDown(Child, Msg);
        if (Consumed) {
            return true;
        }
    }
    
    return false;
}

bool MsgTo(obj& Obj, msg& Msg) {
    int TagCount = hmlen(Obj.Tags);
    for (int Index = 0; Index < TagCount; ++Index) {
        tag& Tag = *Obj.Tags[Index].value;
        bool Consumed = Tag.OnGetMsgFn(Tag, Msg);
        if (Consumed) {
            return true;
        }
    }
    return false;
}

bool OnGetMsg(tag& tag, msg& Msg) {
    return false;
}

obj* GetObj(const tag& Tag) {
    return hmget(TomCtx.ObjMap, Tag.ObjId);
}

const char* TagTypeToCString(tag_type Type) {
    switch (Type) {
        case NPC: return "Npc";
        case FLIPBOOK: return "Flipbook";
        case PLAYER: return "Player";
        case SIM: return "Sim";
        case CAMERA: return "Camera";
        case RIG: return "Rig";
        default:
            return "Unnamed";
    }
}