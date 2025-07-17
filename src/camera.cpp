#include "camera.h"
#include "context.h"
#include "raymath.h"
#include "stb_ds.h"
#include "tray.h"

tag* MakeCameraTag() {
    camera_tag* CameraTag = (camera_tag*) MakeAlloc<camera_tag>();
    *CameraTag = camera_tag{};
    CameraTag->DrawFn = CameraTagDraw;
    CameraTag->TickFn = CameraTagTick;
    CameraTag->OnGetMsgFn = OnCameraTagGetMsg;
    CameraTag->Type = CAMERA;
    return CameraTag;
}

void CameraTagTick(tag& Tag) {
    camera_tag& CameraTag = (camera_tag&) Tag;

    Vector2 TargetPosition = GetGlobalPos(*hmget(TomCtx.ObjMap, CameraTag.TargetId));
    TargetPosition = Vector2Add(
        TargetPosition, 
        CameraTag.SecondaryId != -1 ? 
            Vector2Multiply(
                Vector2Subtract(
                    GetGlobalPos(*hmget(TomCtx.ObjMap, CameraTag.SecondaryId)),
                    TargetPosition
                ),
                (Vector2){0.5f, 0.5f}
            ) : Vector2Zero()
    );
    CameraTag.Camera.target = Vector2Lerp(CameraTag.Camera.target, TargetPosition, GetFrameTime() * 4);
    CameraTag.Mouse = GetScreenToWorld2D(GetMousePosition(), CameraTag.Camera);
}

void CameraTagDraw(const tag& Tag) {
    camera_tag& CameraTag = (camera_tag&) Tag;
    if (CameraTag.SecondaryId > -1) {

        obj* SecondaryTarget = hmget(TomCtx.ObjMap, CameraTag.SecondaryId);
        Vector2 TargetPos = GetGlobalPos(*SecondaryTarget);
        DrawEllipseLines(
            TargetPos.x,
            TargetPos.y,
            30.0f + sin(GetTime() * 5) * 10.0f,
            15.0f + sin(GetTime() * 5) * 5.0f,
            (Color){255,255,255, 187.5f + 55.0f * sin(GetTime() * 5.0f)}
        );
    }

    EndMode2D();

    if (CameraTag.SecondaryId > -1) { 
        obj* SecondaryTarget = hmget(TomCtx.ObjMap, CameraTag.SecondaryId);
        DrawText("TAGS:", GetScreenWidth() - 100, GetScreenHeight() - 65, 16, GRAY);
        for (int Index = 0; Index < SecondaryTarget->Tags->Amt; ++Index) { 
            tag* CurrentTag = (*SecondaryTarget->Tags)[Index];
            DrawText(
                TagTypeToCString(CurrentTag->Type),
                GetScreenWidth() - 90,
                GetScreenHeight() - 50 + Index * 14,
                14,
                GRAY
            );
        }
    }

    BeginMode2D(CameraTag.Camera);
}

bool OnCameraTagGetMsg(tag& Tag, msg& Msg) {
    return false;
}

void SetActiveCamera(camera_tag* CameraTag) {
    TomCtx.SimTag->ActiveCamera = CameraTag;
}

