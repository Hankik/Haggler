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
    int Id = -1;
    obj_state State = obj_state::ALIVE;
    obj *Parent = nullptr;
    tray<tag *> *Tags = nullptr;
    tray<obj *> *Children = nullptr;
    bool Visible = true;
};

void ObjTick(obj *Obj);
void ObjDraw(const obj &Obj);
void AddChild(obj *Obj, obj *newChild);
void AddTag(obj *Obj, tag *newTag);
void RemoveObjFromParent(obj *Obj);

struct tag
{
    tag_type Type = tag_type::EMPTY;
    obj *Obj = nullptr;
    bool Visible = true;
};

void TagTick(tag *Tag);
void TagDraw(const tag &Tag);
bool OnGetMsg(const msg &Msg);
void RemoveTagFromObj(tag *Tag);
