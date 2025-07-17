
#include "npc.h"
#include "context.h"
#include "tray.h"
#include "flipbook.h"

obj* MakeNpcObj() {
    obj* NpcObj = MakeObj();

    NpcObj->Children = MakeTray<obj*>(1);
    NpcObj->Tags = MakeTray<tag*>(2);

    obj* ObjsToAdd[] = {
        MakeFlipbookObj()
    };
    TryAddObjs(*NpcObj, ArrayToTray(ObjsToAdd));

    tag* TagsToAdd[] = {
        MakeNpcTag(),
        MakeCollisionTag(),
    };
    TryAddTags(*NpcObj, ArrayToTray(TagsToAdd));
    return NpcObj;
}

tag* MakeNpcTag(){
    npc_tag* NpcTag =  (npc_tag*) MakeAlloc<npc_tag>();
    *NpcTag = npc_tag{};
    NpcTag->DrawFn = NpcTagDraw;
    NpcTag->TickFn = NpcTagTick;
    NpcTag->OnGetMsgFn = OnNpcGetMsg;
    NpcTag->Type = NPC;
    return NpcTag;
}

void NpcTagTick(tag& Tag){

}

void NpcTagDraw(const tag& Tag) {
    obj* NpcObj = GetObj(Tag);
    DrawCircleV(GetGlobalPos(*NpcObj), 2.0f, GREEN);
}

bool OnNpcGetMsg(tag& Tag, msg& Msg) {

    return false;
}