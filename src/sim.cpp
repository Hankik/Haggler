#include "sim.h"
#include "tray.h"
#include "msg.h"
#include "tom.h"
#include "player.h"
#include "button.h"
#include "context.h"

obj *MakeSimObj()
{
    obj *Sim = MakeObj();
    Sim->Children = MakeTray<obj *>(3);
    Sim->Tags = MakeTray<tag *>(1);
    sim_tag* SimTag = MakeSimTag();
    tag *TagsToAdd[] = {
        SimTag,
    };
    
    obj* ButtonObj = MakeButtonObj();
    ButtonObj->Position = (Vector2){60, 40};
    button_tag* ButtonTag = (button_tag*) TryGetObjTag(*ButtonObj, BUTTON);
    ButtonTag->IsHudElement = false;

    TryAddTags(*Sim, ArrayToTray(TagsToAdd));

    TomCtx.SimTag = SimTag;
    obj* ObjsToAdd[] = {
        MakePlayerObj(),
        ButtonObj,
    };
    TryAddObjs(*Sim, ArrayToTray(ObjsToAdd));
    return Sim;
}

sim_tag *MakeSimTag()
{
    sim_tag* SimTag = (sim_tag *)BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(sim_tag));
    SimTag->TickFn = SimTagTick;
    SimTag->DrawFn = SimTagDraw;
    SimTag->OnGetMsgFn = OnSimGetMsg;
    SimTag->Visible = true;
    SimTag->Type = SIM;

    SimTag->PlayerWalkFrames = MakeTray<Texture2D>(8);
    for (int Index = 0; Index < SimTag->PlayerWalkFrames->Cap; ++Index) {
        TrayAdd(SimTag->PlayerWalkFrames, LoadTexture(TextFormat("data/robot/walk%d.png", Index)));  
    }

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
    // if (Msg.Type == msg_type::KEY_PRESS_MSG)
    // {
    //     key_press_msg &KeyPressMsg = (key_press_msg &)Msg;
    //     printf("%c\n", KeyPressMsg.Key);
    // }
    return false;
}