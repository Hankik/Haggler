#pragma once

#include "tom.h"
#include "camera.h"
#include "raymath.h"
#include "collision.h"

obj* MakeSimObj();


struct ground_click_data {
    Vector2 Position = Vector2Zero();
    float DrawDuration = 1.0f;
    float DrawTimer = 1.0f;
    float GrowRadius = 40.0f;
};

// struct texture_data {
//     tray<Texture2D>* PlayerWalkFrames;
// };

struct sim_tag : tag {
    //texture_data* TextureData;
    int CollisionIdCounter = 0;
    struct {int key; collision_tag* value; } * CollisionTagRegistry = nullptr;
    camera_tag* ActiveCamera = nullptr;
    tray<ground_click_data>* GroundClicks;
    int GroundClickIndex = 0;
};

sim_tag* MakeSimTag();

void SimPhysicsTick(sim_tag& SimTag);
void SimTagTick(tag& SimTag);
void SimTagDraw(const tag& SimTag);
bool OnSimGetMsg(tag& SimTag, msg& Msg);