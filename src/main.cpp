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

   float f;
   struct { float key; char value; } *hash = NULL;
   f=10.5; hmput(hash, f, 'h');
   f=20.4; hmput(hash, f, 'e');
   f=50.3; hmput(hash, f, 'l');
   f=40.6; hmput(hash, f, 'X');
   f=30.9; hmput(hash, f, 'o');

    obj *Sim = MakeSimObj();
    sim_tag* SimTag = (sim_tag*) TryGetObjTag(*Sim, SIM);

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