#pragma once

#include "tom.h"
#include "camera.h"

obj* MakeSimObj();

struct sim_tag : tag {
    tray<Texture2D>* PlayerWalkFrames;
    camera_tag* ActiveCamera = nullptr;
};

sim_tag* MakeSimTag();

void SimTagTick(tag& SimTag);
void SimTagDraw(const tag& SimTag);
bool OnSimGetMsg(tag& SimTag, msg& Msg);