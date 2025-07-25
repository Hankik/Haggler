#include "player.h"
#include "tray.h"
#include "msg.h"
#include "rig.h"
#include "context.h"
#include "sim.h"
#include "camera.h"
#include "stb_ds.h"

obj *MakePlayerObj()
{
    obj *Player = MakeObj();
    Player->Children = MakeTray<obj *>(1);
    obj* RigObj = MakeRigObj();
    RigObj->LocalPos = (Vector2){0, -64};
    rig_tag* RigTag = (rig_tag*) GetObjTag(*RigObj, RIG);
    RigTag->BodyAnims[(int)body_state::IDLE] = "robot_anim";
    SetBodyState(*RigTag, body_state::IDLE);
    RigTag->MouthAnims[(int)mouth_state::IDLE] = "mouthdefault_anim";
    SetMouthState(*RigTag, mouth_state::IDLE);
    RigTag->EyeAnims[(int)eye_state::IDLE] = "eyedefault_anim";
    SetEyeState(*RigTag, eye_state::IDLE);

    obj* ObjsToAdd[] = {
        RigObj
    };
    TryAddObjs(*Player, ArrayToTray(ObjsToAdd));
    
    camera_tag* CameraTag = (camera_tag*) MakeCameraTag();
    CameraTag->TargetId = Player->Id;
    SetActiveCamera(CameraTag);
    tag *TagsToAdd[] = {
        MakePlayerTag(),
        CameraTag,
        MakeCollisionTag(),
    };
    AddTags(*Player, ArrayToTray(TagsToAdd));
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

void PlayerTagTick(tag &Tag) {
    player_tag& PlayerTag = (player_tag&) Tag;

    if (PlayerTag.CurrentMove.IsMoving) {
        if (PlayerTag.CurrentMove.MoveTimer <= PlayerTag.CurrentMove.TimeToReachTarget) {
            PlayerTag.CurrentMove.MoveTimer += GetFrameTime()*1000;
            float MoveAmount = PlayerTag.CurrentMove.MoveTimer / PlayerTag.CurrentMove.TimeToReachTarget;
            GetObj(PlayerTag)->LocalPos = Vector2Lerp( // TODO(hank): create a more readable setPosition function
                PlayerTag.CurrentMove.StartPos, 
                PlayerTag.CurrentMove.TargetPos,
                MoveAmount
            );
        } else {
            PlayerTag.CurrentMove.IsMoving = false;
        }
    }
}

void PlayerTagDraw(const tag &PlayerTag) {
    obj *ParentObj = GetObj(PlayerTag);
    Vector2 PlayerPos = GetGlobalPos(*ParentObj);
    DrawCircleV(ParentObj->LocalPos, 1, RED); // red dot at player origin

    DrawCircle(
        PlayerPos.x,
        PlayerPos.y - 64,
        8,
        WHITE
    );

    camera_tag* ActiveCamera = TomCtx.SimTag->ActiveCamera;

    Vector2 DirectionToMouse = Vector2Normalize(
        Vector2Subtract(
            ActiveCamera->SecondaryId == -1 ? 
                ActiveCamera->Mouse : 
                GetGlobalPos(*hmget(TomCtx.ObjMap, ActiveCamera->SecondaryId)),
            Vector2Add(PlayerPos, (Vector2){0,-64})
        )
    );

    DrawCircle(
        PlayerPos.x + DirectionToMouse.x * 2,
        PlayerPos.y - 64 + DirectionToMouse.y * 2,
        5,
        BLACK                
    );
}

bool OnPlayerGetMsg(tag &Tag, msg &Msg) {
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

            if (MousePress.Button == MOUSE_BUTTON_MIDDLE) {
                float CurrentNearest = FLT_MAX;
                obj* NearestObj = nullptr;
                camera_tag* ActiveCamera = TomCtx.SimTag->ActiveCamera;
                
                for (int Index = 0; Index < hmlen(TomCtx.ObjMap); ++Index) {
                    obj* Obj = TomCtx.ObjMap[Index].value;
                    float NextDist = Vector2Distance(GetGlobalPos(*Obj), ActiveCamera->Mouse);

                    if (NextDist < CurrentNearest) {
                        CurrentNearest = NextDist;
                        NearestObj = Obj;
                    }
                    
                }
                if (NearestObj && NearestObj->Id != ActiveCamera->SecondaryId) {
                    ActiveCamera->SecondaryId = NearestObj->Id;
                } else {
                    ActiveCamera->SecondaryId = -1;
                }
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