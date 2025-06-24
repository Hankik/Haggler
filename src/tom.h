#include "raylib.h"

#pragma once

struct tag;
struct obj;

enum tag_type
{
    EMPTY,
};

enum msg_type
{

};

struct msg
{
    msg_type Type;
};

bool MsgUp(const obj &From, msg *Msg);
bool MsgDown(const obj &From, msg *Msg);
bool MsgTo(const obj &To, msg *Msg);

enum obj_state
{
    ALIVE,
    ASLEEP,
    DEAD,
};

struct obj
{

    int Id;
    obj_state State;
    bool Visible;
    int Parent;
    int TagTrayHandle; // im going to want my own array type
    int KidTrayHandle;
};

void ObjTick(obj *Obj);
void ObjDraw(const obj &Obj);
bool TryAddKid(obj *Obj, int KidHandle);
bool TryAddTag(obj *Obj, int TagHandle);
void RemoveObjFromParent(obj *Obj);

struct tag
{
    tag_type Type = tag_type::EMPTY;
    obj* Obj = nullptr;
    bool Visible = true;
};

void TagTick(tag *Tag);
void TagDraw(const tag &Tag);
bool OnGetMsg(const msg &Msg);
void RemoveTagFromObj(tag *Tag);
