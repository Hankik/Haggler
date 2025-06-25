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

#include "globals.h"
#include "tom.h"
#include "tray.h"
#include "player.h"
#include <iostream>

//-----------------------------------------=-------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

using namespace Globals;

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    BuddyAllocatorInit(&BuddyAlloc, BackingBuffer, ALLOCATOR_SIZE, 16);

    tray<tag>* Test = MakeTray<tag>(&BuddyAlloc, 3);
    tag Tag;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        float DeltaTime = GetFrameTime();

        char Key = GetCharPressed();
        while (Key > 0)
        {
            Key = GetCharPressed();
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