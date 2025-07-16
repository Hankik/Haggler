#pragma once

#include "tom.h"

struct camera_tag : tag {
    Camera2D Camera {
        .offset = Vector2{GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f},
        .target = Vector2{0,0},
        .rotation = 0.0f,
        .zoom = 1.0f
    };
    int TargetId = -1;
    Vector2 Mouse {0,0};
};

tag* MakeCameraTag();
void CameraTagTick(tag& Tag);
void CameraTagDraw(const tag& Tag);
bool OnCameraTagGetMsg(tag& Tag, msg& Mgs);
void SetActiveCamera(camera_tag* CameraTag);