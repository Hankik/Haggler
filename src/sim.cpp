#include "sim.h"
#include "tray.h"
#include "msg.h"
#include "tom.h"
#include "player.h"
#include "button.h"
#include "context.h"
#include "npc.h"

obj *MakeSimObj()
{
    obj *Sim = MakeObj();
    Sim->Children = MakeTray<obj *>(3);
    sim_tag* SimTag = MakeSimTag();
    tag *TagsToAdd[] = {
        SimTag,
    };
    TomCtx.SimTag = SimTag;

    AddTags(*Sim, ArrayToTray(TagsToAdd));
    obj* NpcObj = MakeNpcObj();
    NpcObj->LocalPos = (Vector2){50, 50};

    obj* ObjsToAdd[] = {
        MakePlayerObj(),
        NpcObj,
    };
    TryAddObjs(*Sim, ArrayToTray(ObjsToAdd));
    return Sim;
}

sim_tag *MakeSimTag()
{
    sim_tag* SimTag = (sim_tag *)BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(sim_tag));
    *SimTag = sim_tag{};
    SimTag->TickFn = SimTagTick;
    SimTag->DrawFn = SimTagDraw;
    SimTag->OnGetMsgFn = OnSimGetMsg;
    SimTag->Visible = true;
    SimTag->Type = SIM;
    SimTag->GroundClicks = MakeTray<ground_click_data>(4);
    for (int Index = 0; Index < SimTag->GroundClicks->Cap; ++Index) {
        TrayAdd(SimTag->GroundClicks, ground_click_data{});
    }
    //texture_data * TextureData = (texture_data*) MakeAlloc<texture_data>();
    

    // TextureData->PlayerWalkFrames = MakeTray<Texture2D>(8);
    // for (int Index = 0; Index < TextureData->PlayerWalkFrames->Cap; ++Index) {
    //     TrayAdd(TextureData->PlayerWalkFrames, LoadTexture(TextFormat("data/robot/walk%d.png", Index)));  
    // }
    // SimTag->TextureData = TextureData;
    return (sim_tag *)SimTag;
}

void SimPhysicsTick(sim_tag& SimTag) {

    int RegisterLength = hmlen(SimTag.CollisionTagRegistry);
    for (int Index = 0; Index < RegisterLength; ++Index) { 
        collision_tag* TagToReset = SimTag.CollisionTagRegistry[Index].value;
        hmfree(TagToReset->Collisions);
    }
    for (int I = 0; I < RegisterLength; ++I) {
        auto ColTagA = SimTag.CollisionTagRegistry[I].value;

        for (int J = 0; J < RegisterLength; ++J) {
            auto ColTagB = SimTag.CollisionTagRegistry[J].value;
            
            if (ColTagA->CollisionId != ColTagB->CollisionId) {

                if (IsColliding(*ColTagA, *ColTagB)) {
                    hmput(ColTagA->Collisions, ColTagB->CollisionId, ColTagB);
                    hmput(ColTagB->Collisions, ColTagA->CollisionId, ColTagB);
                }
            }
        }
    }
}

void SimTagTick(tag &Tag)
{
    
}

void SimTagDraw(const tag &Tag)
{
    sim_tag& SimTag = (sim_tag&) Tag;
    for (int Index = 0; Index < SimTag.GroundClicks->Amt; ++Index) {
        ground_click_data GroundClickData = (*SimTag.GroundClicks)[Index];

        float ClickDrawTime = GroundClickData.DrawTimer;
        float ClickDrawDuration = GroundClickData.DrawDuration;
        
        if (ClickDrawTime < ClickDrawDuration) {
            float DrawRadius = Lerp(0, GroundClickData.GrowRadius, ClickDrawTime / ClickDrawDuration);
            DrawEllipseLines(
                GroundClickData.Position.x,
                GroundClickData.Position.y,
                DrawRadius,
                DrawRadius * 0.75,
                (Color) {255, 255, 255, 255 * (1 - ClickDrawTime / ClickDrawDuration)}
            );

            (*SimTag.GroundClicks)[Index].DrawTimer += GetFrameTime();
        }
    }
}

bool OnSimGetMsg(tag &Tag, msg &Msg)
{
    sim_tag& SimTag = (sim_tag&) Tag;
    switch (Msg.Type) {
        case MOUSE_PRESS_MSG: {
            mouse_press_msg& MousePress = (mouse_press_msg&) Msg;
            if (MousePress.Button == MOUSE_LEFT_BUTTON) {

                // for (int Index = 0; Index < Tag.Obj->Children->Amt; ++Index) {
                //     obj* Child = (*SimTag.Obj->Children)[Index];
                //     bool Consumed = MsgDown(*Child, Msg);
                //     if (Consumed) {
                //         return true;
                //     }
                // }
                ground_click_data GroundClick{};
                GroundClick.Position = SimTag.ActiveCamera->Mouse;
                GroundClick.DrawTimer = 0.0f;
                (*SimTag.GroundClicks)[SimTag.GroundClickIndex] = GroundClick;
                SimTag.GroundClickIndex++;
                if (SimTag.GroundClickIndex >= SimTag.GroundClicks->Amt) SimTag.GroundClickIndex = 0;
            }
        } break;
        default: break;
    }
    return false;
}