#pragma once

#include "tom.h"
#include "raylib.h"

obj* MakeFlipbookObj();
tag* MakeFlipbookTag();

extern tray<Texture2D>* WalkFrames;

struct flipbook_tag : tag {
    tray<Texture2D>* Frames = nullptr;
    int CurrentFrame = 0;
    float NextFrameDelay = 0.1;
    float NextFrameTimer = 0;
};

void FlipbookTagTick(tag& Tag);
void FlipbookTagDraw(const tag& Tag);
bool OnFlipbookGetMsg(tag& Tag, msg& Msg);