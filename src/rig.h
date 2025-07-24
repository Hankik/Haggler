#pragma once

#include "tom.h"

obj* MakeRigObj();

enum class body_state {
    IDLE = 0,
    MOVING,
    AMOUNT
};

enum class eye_state {
    IDLE = 0,
    SLEEPING,
    ANGRY,
    HAPPY,
    TIRED,
    SCARED,
    DEAD,
    AMOUNT,
};

enum class mouth_state {
    IDLE = 0,
    TALKING,
    HAPPY,
    FROWN,
    LAUGHING,
    WHISTLING,
    EATING,
    AMOUNT,
};



struct rig_tag : tag {
    eye_state   EyeState   = eye_state::IDLE;
    body_state  BodyState  = body_state::IDLE;
    mouth_state MouthState = mouth_state::IDLE;
    const char * EyeAnims[(int) eye_state::AMOUNT]     = {"default"};  // use these strings to access AnimMap anim
    const char * BodyAnims[(int) body_state::AMOUNT]   = {"default"};
    const char * MouthAnims[(int) mouth_state::AMOUNT] = {"default"};
    int EyeObjId = -1;
    int BodyObjId = -1;
    int MouthObjId = -1;
};



tag* MakeRigTag();

void RigTagTick(tag& Tag);
void RigTagDraw(const tag& Tag);
bool OnRigGetMsg(tag& Tag, msg& Msg);
void SetEyeState(rig_tag& RigTag, eye_state NewState);
void SetMouthState(rig_tag& RigTag, mouth_state NewState);
void SetBodyState(rig_tag& RigTag, body_state NewState);

