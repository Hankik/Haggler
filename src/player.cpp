#include "player.h"
#include "tray.h"
#include "msg.h"
#include "flipbook.h"
#include "context.h"
#include "sim.h"
#include "camera.h"

obj *MakePlayerObj()
{
    obj *Player = MakeObj();
    Player->Children = MakeTray<obj *>(1);
    Player->Tags = MakeTray<tag *>(3);
    sim_tag* SimTag = TomCtx.SimTag;
    obj* FlipbookObj = MakeFlipbookObj();
    FlipbookObj->LocalPos = (Vector2){0, -64};
    flipbook_tag* FlipbookTag = (flipbook_tag*) TryGetObjTag(*FlipbookObj, FLIPBOOK);
    FlipbookTag->Frames = SimTag->PlayerWalkFrames;
    obj* ObjsToAdd[] = {
        FlipbookObj
    };
    TryAddObjs(*Player, ArrayToTray(ObjsToAdd));
    
    camera_tag* CameraTag = (camera_tag*) MakeCameraTag();
    CameraTag->TargetId = Player->Id;
    SetActiveCamera(CameraTag);

    tag *TagsToAdd[] = {
        MakePlayerTag(),
        CameraTag,
    };
    TryAddTags(*Player, ArrayToTray(TagsToAdd));
    return Player;
}

tag *MakePlayerTag()
{
    player_tag *PlayerTag = (player_tag *)MakeAlloc<player_tag>();
    *PlayerTag = player_tag{};
    PlayerTag->TickFn = PlayerTagTick;
    PlayerTag->DrawFn = PlayerTagDraw;
    PlayerTag->OnGetMsgFn = OnPlayerGetMsg;
    PlayerTag->Type = PLAYER;
    return PlayerTag;
}

void PlayerTagTick(tag &Tag)
{
    player_tag& PlayerTag = (player_tag&) Tag;
   
    if (PlayerTag.CurrentMove.IsMoving) {
        if (PlayerTag.CurrentMove.MoveTimer <= PlayerTag.CurrentMove.TimeToReachTarget) {
            PlayerTag.CurrentMove.MoveTimer += GetFrameTime()*1000;
            float Alpha = PlayerTag.CurrentMove.MoveTimer / PlayerTag.CurrentMove.TimeToReachTarget;
            GetObj(PlayerTag)->LocalPos = Vector2Lerp(
                PlayerTag.CurrentMove.StartPos, 
                PlayerTag.CurrentMove.TargetPos,
                Alpha
            );
        } else {
            PlayerTag.CurrentMove.IsMoving = false;
        }
    }
}
void PlayerTagDraw(const tag &PlayerTag)
{
    obj *ParentObj = GetObj(PlayerTag);
    DrawCircleV(ParentObj->LocalPos, 1, RED);
}

bool OnPlayerGetMsg(tag &Tag, msg &Msg)
{
    player_tag& PlayerTag = (player_tag&) Tag;

    switch (Msg.Type) {
        case KEY_PRESS_MSG: {
        } break;
        
        case KEY_RELEASE_MSG: {
        } break;
        case MOUSE_PRESS_MSG: {
            mouse_press_msg& MousePress = (mouse_press_msg&) Msg;
            obj* PlayerObj = GetObj(PlayerTag);
            if (MousePress.Button == MOUSE_BUTTON_LEFT) {
                PlayerMoveTo(PlayerTag, TomCtx.SimTag->ActiveCamera->Mouse);
            } else if (
                MousePress.Button == MOUSE_BUTTON_RIGHT &&
                CheckCollisionPointRec(
                    TomCtx.SimTag->ActiveCamera->Mouse,
                    (Rectangle) {
                        .x = PlayerObj->LocalPos.x,
                        .y = PlayerObj->LocalPos.y,
                        .width = 100.0f,
                        .height = 200.0f,
                }))
            {
                TogglePlayerMenu(PlayerTag);
            }
        }
    }
    return false;
}

void TogglePlayerMenu(player_tag& PlayerTag) {
    printf("Open Player Menu\n");
}

void PlayerMoveTo(player_tag& PlayerTag, Vector2 ToPos) {
    move_to_data MoveTo{};
    MoveTo.IsMoving = true;
    MoveTo.StartPos = GetObj(PlayerTag)->LocalPos;
    MoveTo.TargetPos = ToPos;
    float MoveDist = Vector2Distance(MoveTo.StartPos, ToPos);
    if (MoveDist > 0.1f) {
        MoveTo.TimeToReachTarget = MoveDist * MoveTo.MillisPerPixel;
        
        PlayerTag.CurrentMove = MoveTo;
    }     
}