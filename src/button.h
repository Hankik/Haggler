
#pragma once 

#include "tom.h"


enum button_state {
    IDLE,
    HOVERED,
    PRESSED
};

struct button_tag : tag {
    Vector2 Offset{0};
    Vector2 Size{0};
    button_state ButtonState = button_state::IDLE;
    bool IsHudElement = true;
};

obj* MakeButtonObj();
tag* MakeButtonTag();
void ButtonTagTick(tag& Tag);
void ButtonTagDraw(const tag& Tag);
bool OnButtonGetMsg(tag& Tag, msg& Msg);

