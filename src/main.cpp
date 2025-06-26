/*******************************************************************************************
 *
 *   raylib [core] example - Basic window
 *
 *   Welcome to raylib!
 *
 *   To test examples, just press F6 and execute 'raylib_compile_execute' script
 *
 ********************************************************************************************/

#include "raylib.h"
extern "C"
{
#include "allocators.h"
}

#include "context.h"
#include "tom.h"
#include "tray.h"
#include "player.h"
//#include "msg.h"
#include <iostream>

//-----------------------------------------=-------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int ScreenWidth = 800;
    const int ScreenHeight = 450;

    InitWindow(ScreenWidth, ScreenHeight, "Haggler");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    BuddyAllocatorInit(TomCtx.BuddyAlloc, BackingBuffer, ALLOCATOR_SIZE, 16);

    obj* Sim = MakeSimObj();

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose() && Sim) // Detect window close button or ESC key
    {
        float DeltaTime = GetFrameTime();

        char Key = GetCharPressed();
        while (Key > 0)
        {
            Key = GetCharPressed();

            // key_press_msg KeyPressMsg;
            // KeyPressMsg.Key = Key;
            // MsgDown(*Sim, KeyPressMsg);
        }

        ClearBackground(BLACK);

        BeginDrawing();
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}