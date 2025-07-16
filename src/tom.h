
#pragma once

#include "raylib.h"
#include "msg.h"

struct tag;
struct obj;
template<typename T>
struct tray;

enum tag_type
{
    EMPTY,
    SIM,
    PLAYER,
    BUTTON,
    FLIPBOOK,
};

bool MsgUp(obj &From, msg& Msg);
bool MsgDown(obj &From, msg& Msg);
bool MsgTo(obj &To, msg &Msg);
obj* GetObj(const tag& Tag);

enum obj_state
{
    ALIVE,
    ASLEEP,
    DEAD,
};

struct obj
{
    int Id = -1;
    Vector2 LocalPos;
    obj_state State = obj_state::ALIVE;
    int ParentId = -1;
    tray<tag *> *Tags;
    tray<obj *> *Children;
    bool Visible = true;
    bool IsMasked = false;
};

obj* MakeObj();
tag* MakeTag(tag_type Type);

Vector2 GetGlobalPos(const obj& Obj);
void ObjTick(obj& Obj);
void ObjDraw(const obj &Obj);
tag* TryGetObjTag(const obj& Obj, tag_type Type);

bool TryAddObjs(obj & Obj, const tray<obj*>& newObjs);
bool TryAddTags(obj & Obj, const tray<tag*>& newTags);

struct tag
{
    tag_type Type = tag_type::EMPTY;
    int ObjId = -1;
    void (*TickFn)(tag&) = nullptr;
    void (*DrawFn)(const tag&) = nullptr;
    bool (*OnGetMsgFn)(tag& Tag, msg& Msg) = nullptr;
    bool Visible = true;
    bool IsMasked = false;
};


void TagTick(tag &Tag);
void TagDraw(const tag &Tag);
bool OnGetMsg(tag& Tag, msg& Msg);
