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
#include <vector>
#include <string>
#include "actors.h"
#include <iostream>

int NextActorId = 0;
int NextComponentId = 0;
std::array<actor *, 100> Actors;
std::array<component *, 500> Components;

//-----------------------------------------=-------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    actor Player = actor();
    actor Player2 = actor();
    Actors[Player.Id] = &Player;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        float DeltaTime = GetFrameTime();
        Player.Update(DeltaTime);

        BeginDrawing();

        ClearBackground(BLACK);

        // Player.Display();

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}