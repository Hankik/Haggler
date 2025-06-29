#include "camera.h"
#include "context.h"
#include "raymath.h"

tag* MakeCameraTag() {
    camera_tag* CameraTag = (camera_tag*) MakeAlloc<camera_tag>();
    *CameraTag = camera_tag{};
    CameraTag->DrawFn = CameraTagDraw;
    CameraTag->TickFn = CameraTagTick;
    CameraTag->OnGetMsgFn = OnCameraTagGetMsg;
    return CameraTag;
}

void CameraTagTick(tag& Tag) {
    camera_tag& CameraTag = (camera_tag&) Tag;

    Vector2 TargetPosition = CameraTag.Target->Position;
    CameraTag.Camera.target = Vector2Lerp(CameraTag.Camera.target, TargetPosition, GetFrameTime() * 4);
    CameraTag.Mouse = GetScreenToWorld2D(GetMousePosition(), CameraTag.Camera);
}

void CameraTagDraw(const tag& Tag) {

}

bool OnCameraTagGetMsg(tag& Tag, msg& Msg) {
    return false;
}

void SetActiveCamera(camera_tag* CameraTag) {
    TomCtx.SimTag->ActiveCamera = CameraTag;
}

