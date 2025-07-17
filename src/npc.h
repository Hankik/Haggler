
#pragma once

#include "tom.h"

obj* MakeNpcObj();


struct npc_tag : tag {
};

tag* MakeNpcTag();

void NpcTagTick(tag& Tag);
void NpcTagDraw(const tag& Tag);
bool OnNpcGetMsg(tag& Tag, msg& Msg);
