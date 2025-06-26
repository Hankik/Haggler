#include "raylib.h"
#include "context.h"

#pragma once

struct tag;
struct obj;
template<typename T>
struct tray;

enum tag_type
{
    EMPTY,
};

enum msg_type
{
    KEY_PRESS_MSG
};

struct msg
{
    msg_type Type;
};

bool MsgUp(const obj &From, const msg& Msg);
bool MsgDown(const obj &From, const msg& Msg);
bool MsgTo(const obj &To, const msg &Msg);

enum obj_state
{
    ALIVE,
    ASLEEP,
    DEAD,
};

struct obj
{
    int Id = -1;
    obj_state State = obj_state::DEAD;
    obj *Parent = nullptr;
    tray<tag *> *Tags;
    tray<obj *> *Children;
    bool Visible = true;
};

obj* MakeObj();
obj* MakeSimObj();

void ObjTick(obj *Obj);
void ObjDraw(const obj &Obj);
bool TryAddObjs(obj *Obj, const tray<obj*>& newObjs);
bool TryAddTags(obj *Obj, const tray<tag*>& newTags);

struct tag
{
    tag_type Type = tag_type::EMPTY;
    obj *Obj = nullptr;
    void (*TickFn)(tag*) = nullptr;
    void (*DrawFn)(const tag&) = nullptr;
    bool (*OnGetMsgFn)(tag* Tag, const msg& Msg) = nullptr;
    bool Visible = true;
};



void TagTick(tag *Tag);
void TagDraw(const tag &Tag);
bool OnGetMsg(tag* Tag, const msg &Msg);
void RemoveTagFromObj(tag *Tag);
