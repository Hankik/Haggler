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
#include "player.h"
#include "mail.h"
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

    player Player = player();
    actor Actor = actor();
    Player.AddActor(&Actor);
    Actors[Player.Id] = &Player;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        float DeltaTime = GetFrameTime();

        char Key = GetCharPressed();
        while (Key > 0)
        {
            key_pressed_mail KeyPressedMail(Key);
            Player.SendDown(KeyPressedMail);
            Key = GetCharPressed();
        }

        Player.Update(DeltaTime);

        ClearBackground(BLACK);

        BeginDrawing();

        Player.Display();

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}