#include "player.h"
#include "tray.h"
#include "msg.h"
#include "raymath.h"

obj *MakePlayerObj()
{
    obj *Player = MakeObj();
    Player->Children = MakeTray<obj *>(0);
    Player->Tags = MakeTray<tag *>(1);
    tag *TagsToAdd[] = {
        MakePlayerTag(),
    };
    TryAddTags(*Player, ArrayToTray(TagsToAdd));
    return Player;
}

tag *MakePlayerTag()
{
    player_tag *PlayerTag = (player_tag *)MakeAlloc<player_tag>();
    PlayerTag->TickFn = PlayerTagTick;
    PlayerTag->DrawFn = PlayerTagDraw;
    PlayerTag->OnGetMsgFn = OnPlayerGetMsg;
    PlayerTag->Visible = true;
    PlayerTag->Type = PLAYER;
    PlayerTag->MovingLeft = PlayerTag->MovingRight = PlayerTag->MovingDown = PlayerTag->MovingUp = false;
    return PlayerTag;
}

bool IsMoving(player_tag& PlayerTag) {
    return PlayerTag.MovingUp || PlayerTag.MovingDown || PlayerTag.MovingRight || PlayerTag.MovingLeft;
}

void PlayerTagTick(tag &Tag)
{
    player_tag& PlayerTag = (player_tag&) Tag;
   
    if (IsMoving(PlayerTag)) {
        Vector2 Direction;
        if (PlayerTag.MovingRight && !PlayerTag.MovingLeft) { Direction.x++; }
        if (!PlayerTag.MovingRight && PlayerTag.MovingLeft) { Direction.x--; }
        if (PlayerTag.MovingUp && !PlayerTag.MovingDown) { Direction.y--; }
        if (!PlayerTag.MovingUp && PlayerTag.MovingDown) { Direction.y++; }
        Vector2Normalize(Direction);

        float MoveSpeed = 10;
        obj &ParentObj = *PlayerTag.Obj;
        Vector2 MoveAmt = Vector2Multiply((Vector2){MoveSpeed, MoveSpeed}, Direction);
        ParentObj.Position = Vector2Add(ParentObj.Position, MoveAmt);
    }
}
void PlayerTagDraw(const tag &PlayerTag)
{
    obj *ParentObj = PlayerTag.Obj;
    DrawCircleV(ParentObj->Position, 30, WHITE);
}

bool OnPlayerGetMsg(tag &Tag, msg &Msg)
{
    player_tag& PlayerTag = (player_tag&) Tag;

    switch (Msg.Type) {
        case KEY_PRESS_MSG: {
            key_press_msg &KeyPress = (key_press_msg &)Msg;
            if (KeyPress.Key == KEY_A) { PlayerTag.MovingLeft = true; }
            if (KeyPress.Key == KEY_D) { PlayerTag.MovingRight = true; }
            if (KeyPress.Key == KEY_W) { PlayerTag.MovingUp = true; }
            if (KeyPress.Key == KEY_S) { PlayerTag.MovingDown = true; }
        } break;
        
        case KEY_RELEASE_MSG: {
            key_release_msg &KeyRelease = (key_release_msg&) Msg;
            if (KeyRelease.Key == KEY_A) { PlayerTag.MovingLeft = false; }
            if (KeyRelease.Key == KEY_D) { PlayerTag.MovingRight = false; }
            if (KeyRelease.Key == KEY_W) { PlayerTag.MovingUp = false; }
            if (KeyRelease.Key == KEY_S) { PlayerTag.MovingDown = false; }
        } break;
    }
    return false;
}