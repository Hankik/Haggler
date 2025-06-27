#pragma once

#include "tom.h"

obj* MakePlayerObj();

struct player_tag : tag {
    bool MovingUp = false;
    bool MovingDown = false;
    bool MovingRight = false;
    bool MovingLeft = false;
};

tag* MakePlayerTag();

void PlayerTagTick(tag& PlayerTag);
void PlayerTagDraw(const tag& PlayerTag);
bool OnPlayerGetMsg(tag& PlayerTag, msg& Msg);