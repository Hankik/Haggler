#include "raylib.h"
#include "tom.h"
#include "context.h"
#include "tray.h"
#include "sim.h"
#include "player.h"
#include "button.h"

obj* MakeObj() {
    obj* NewObj = (obj*) BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(obj));
    if (NewObj) {
        NewObj->Id = TomCtx.IdCounter++;
        NewObj->Visible = true;
        return NewObj;
    }
    return nullptr;
}

tag* TryGetObjTag(const obj& Obj, tag_type Type) {

    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        tag* Tag = (*Obj.Tags)[Index];
        if (Tag->Type == Type) {
            return Tag;
        }
    }
    return nullptr; 
}

tag* MakeTag(tag_type Type) {
    switch (Type) {
        case EMPTY: {
            tag* NewTag = (tag*) BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(tag)); 
            if (NewTag) {
                NewTag->TickFn = TagTick;
                NewTag->DrawFn = TagDraw;
                NewTag->OnGetMsgFn = OnGetMsg;
                return NewTag;
            }
        } break;
        case SIM: {
            return MakeSimTag();
        } break;
        case PLAYER: {
            return MakePlayerTag();
        } break;
        case BUTTON: {
            return MakeButtonTag();
        } break;
    }
    return nullptr;
}


void ObjTick(obj& Obj)
{
    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        tag* Tag = (*Obj.Tags)[Index];
        if (Tag) {
            (*Tag).TickFn(*Tag);
        }
    }
    for (int Index = 0; Index < Obj.Children->Amt; ++Index) {
        obj* Child = (*Obj.Children)[Index];
        if (Child && Child->State == obj_state::ALIVE) {
            ObjTick(*Child);
        }
    }
}

void ObjDraw(const obj& Obj)
{
    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        tag *Tag = (*Obj.Tags)[Index];
        if (Tag && Tag->Visible)
        { 
            (*Tag).DrawFn(*Tag);
        }
    }
    for (int Index = 0; Index < Obj.Children->Amt; ++Index) {
        obj* Child = (*Obj.Children)[Index];
        if (Child && Child->Visible) {
            ObjDraw(*Child);
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
            ObjToAdd->Parent = &Obj;
            TrayAdd(Obj.Children, ObjToAdd);
        }
        return true;
    } else {
        return false;
    }
}

bool TryAddTags(obj& Obj, const tray<tag*>& NewTags) {
    if (Obj.Tags->Amt + NewTags.Amt <= Obj.Tags->Cap) {
        for (int Index = 0; Index < NewTags.Amt; ++Index) {
            tag* TagToAdd = NewTags[Index];
            TagToAdd->Obj = &Obj;
            TrayAdd(Obj.Tags, TagToAdd);
        }
        return true;
    } else {
        return false;
    }
}

bool MsgUp(obj& Obj, msg& Msg) {
    tray<tag*> Tags = *Obj.Tags;
    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        tag& Tag = *Tags[Index];
        bool Consumed = Tag.OnGetMsgFn(Tag, Msg);
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

bool MsgDown(obj& Obj, msg& Msg) {
    tray<tag*> Tags = *Obj.Tags;
    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        tag& Tag = *Tags[Index];
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
    tray<tag*> Tags = *Obj.Tags;
    for (int Index = 0; Index < Obj.Tags->Amt; ++Index) {
        tag& Tag = *Tags[Index];
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