#include "sim.h"
#include "tray.h"
#include "msg.h"
#include "tom.h"
#include "player.h"
#include "button.h"

obj *MakeSimObj()
{
    obj *Sim = MakeObj();
    Sim->Children = MakeTray<obj *>(3);
    Sim->Tags = MakeTray<tag *>(1);
    tag *TagsToAdd[] = {
        MakeSimTag(),
    };
    
    obj* ButtonObj = MakeButtonObj();
    ButtonObj->Position = (Vector2){40, 40};

    TryAddTags(*Sim, ArrayToTray(TagsToAdd));
    obj* ObjsToAdd[] = {
        MakePlayerObj(),
        ButtonObj,
    };
    TryAddObjs(*Sim, ArrayToTray(ObjsToAdd));
    return Sim;
}

sim_tag *MakeSimTag()
{
    tag *SimTag = (tag *)BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(sim_tag));
    SimTag->TickFn = SimTagTick;
    SimTag->DrawFn = SimTagDraw;
    SimTag->OnGetMsgFn = OnSimGetMsg;
    SimTag->Visible = true;
    SimTag->Type = SIM;
    return (sim_tag *)SimTag;
}

void SimTagTick(tag &SimTag)
{
}

void SimTagDraw(const tag &SimTag)
{
}

bool OnSimGetMsg(tag &SimTag, msg &Msg)
{
    if (Msg.Type == msg_type::KEY_PRESS_MSG)
    {
        key_press_msg &KeyPressMsg = (key_press_msg &)Msg;
        printf("%c\n", KeyPressMsg.Key);
    }
    return false;
}