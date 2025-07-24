
#pragma once

#include "tom.h"
#include "context.h"

tag_type TagsToSave[] = {
    PLAYER,
    SIM,
    NPC,
}; 
// TODO(hank): find all children with these types or 
// use msg down with save context and optional save function (I think i Like this one)

struct save_msg : msg {
    void (*SaveObjFn)(const obj& Obj);
};

void MakeSave(const char * SaveName);
void LoadSave(const char * SaveName);