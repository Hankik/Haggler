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

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose() && Sim) // Detect window close button or ESC key
    {
        float DeltaTime = GetFrameTime();

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

        ClearBackground(BLACK);

        ObjTick(*Sim);

        BeginDrawing();

        ObjDraw(*Sim);

        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}