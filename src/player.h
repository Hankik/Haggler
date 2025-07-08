#pragma once

#include "tom.h"
#include "raymath.h"

obj* MakePlayerObj();

struct move_to_data {
    bool IsMoving = false;
    float MillisPerPixel = 2.0f;
    float TimeToReachTarget = 0.0f;
    float MoveTimer = 0.0f;
    Vector2 StartPos = Vector2Zero();
    Vector2 TargetPos = Vector2Zero();
};

struct player_tag : tag {
    move_to_data CurrentMove;
};



tag* MakePlayerTag();

void PlayerTagTick(tag& PlayerTag);
void PlayerTagDraw(const tag& PlayerTag);
bool OnPlayerGetMsg(tag& PlayerTag, msg& Msg);

void TogglePlayerMenu(player_tag& PlayerTag);
void PlayerMoveTo(player_tag& PlayerTag, Vector2 ToPos);