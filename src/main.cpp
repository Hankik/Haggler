/*******************************************************************************************
 *
 *   Hank + Raylib Game Contruction in Progress
 *
 ********************************************************************************************/

#include "raylib.h"
#include "context.h"
#include "tom.h"
#include "msg.h"
#include "sim.h"
#include <cmath>
#include "collision.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int ScreenWidth = 800;
    const int ScreenHeight = 450;
    InitWindow(ScreenWidth, ScreenHeight, "Haggler");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    BuddyAllocatorInit(TomCtx.BuddyAlloc, BackingBuffer, ALLOCATOR_SIZE, 16);

    obj *Sim = MakeSimObj();
    sim_tag* SimTag = (sim_tag*) TryGetObjTag(*Sim, SIM);

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose() && Sim) // Detect window close button or ESC key
    {
        //float DeltaTime = GetFrameTime();

        for (int Button = MOUSE_BUTTON_LEFT; Button <= MOUSE_BUTTON_BACK; ++Button) {
            if (IsMouseButtonPressed(Button)) {
                mouse_press_msg MousePressMsg;
                MousePressMsg.Type = MOUSE_PRESS_MSG;
                MousePressMsg.Button = Button;
                MsgDown(*Sim, MousePressMsg);
            }
        }
        int Key = GetKeyPressed();
        while (Key > 0) {
            key_press_msg KeyPressMsg;
            KeyPressMsg.Key = Key;
            KeyPressMsg.Type = KEY_PRESS_MSG;
            MsgDown(*Sim, KeyPressMsg);
            Key = GetKeyPressed();
        }
        for (Key = 32; Key <= 349; ++Key) { 
            if (IsKeyReleased(Key)) {
                key_release_msg KeyReleaseMsg;
                KeyReleaseMsg.Key = Key;
                KeyReleaseMsg.Type = msg_type::KEY_RELEASE_MSG;
                MsgDown(*Sim, KeyReleaseMsg);
            }
        }
        float CurrentZoom = SimTag->ActiveCamera->Camera.zoom;
        SimTag->ActiveCamera->Camera.zoom = expf(logf(CurrentZoom) + ((float)GetMouseWheelMove()*0.1f));
        ObjTick(*Sim);

        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode2D(SimTag->ActiveCamera->Camera);

            ObjDraw(*Sim);

            EndMode2D();

        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}