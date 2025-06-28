#pragma once

#include "tom.h"

obj* MakeSimObj();

struct sim_tag : tag {
    tray<Texture2D>* PlayerWalkFrames;
};

sim_tag* MakeSimTag();

void SimTagTick(tag& SimTag);
void SimTagDraw(const tag& SimTag);
bool OnSimGetMsg(tag& SimTag, msg& Msg);