/*******************************************************************************************
 *
 *   Hank + Raylib Game Contruction in Progress
 *
 ********************************************************************************************/

#include "raylib.h"
#include "context.h"  // globals
#include "tom.h"      // entities and components (objs and tags)
#include "msg.h"
#include "sim.h"  // top level obj
#include <cmath>
#include "collision.h" 
#include <filesystem>  // For std::filesystem
#include <fstream>     // For file input/output
#include "tray.h"      // small custom array

#define STBDS_REALLOC(context,ptr,size) BuddyRealloc(context, ptr, size)
#define STBDS_FREE(context,ptr)         BuddyFree(context, ptr)
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

void LoadDataFolder();

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int ScreenWidth  = 800;
    const int ScreenHeight = 450;
    InitWindow(ScreenWidth, ScreenHeight, "Haggler");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    BuddyAllocatorInit(TomCtx.BuddyAlloc, BackingBuffer, ALLOCATOR_SIZE, 16);

    Shader BloomShader = LoadShader(0, "data/bloom.fs");
    RenderTexture2D RenderTarget = LoadRenderTexture(ScreenWidth, ScreenHeight);

    LoadDataFolder();

    obj *Sim = MakeSimObj();
    sim_tag* SimTag = (sim_tag*) GetObjTag(*Sim, SIM);

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose() && Sim) // Detect window close button or ESC key
    {
        //float DeltaTime = GetFrameTime();

        for (int Button = MOUSE_BUTTON_LEFT; Button <= MOUSE_BUTTON_BACK; ++Button) {
            if (IsMouseButtonPressed(Button)) {
                mouse_press_msg MousePressMsg;
                MousePressMsg.Type   = MOUSE_PRESS_MSG;
                MousePressMsg.Button = Button;
                MsgDown(*Sim, MousePressMsg);
            }
        }
        int Key = GetKeyPressed();
        while (Key > 0) {
            key_press_msg KeyPressMsg;
            KeyPressMsg.Key  = Key;
            KeyPressMsg.Type = KEY_PRESS_MSG;
            MsgDown(*Sim, KeyPressMsg);
            Key = GetKeyPressed(); // cycles through key events until none left
        }
        for (Key = 32; Key <= 349; ++Key) { 
            if (IsKeyReleased(Key)) {
                key_release_msg KeyReleaseMsg;
                KeyReleaseMsg.Key  = Key;
                KeyReleaseMsg.Type = msg_type::KEY_RELEASE_MSG;
                MsgDown(*Sim, KeyReleaseMsg);
            }
        }
        float CurrentZoom = SimTag->ActiveCamera->Camera.zoom;
        SimTag->ActiveCamera->Camera.zoom = expf(logf(CurrentZoom) + ((float)GetMouseWheelMove()*0.1f));

        SimPhysicsTick(*SimTag);
        ObjTick(*Sim);

        BeginTextureMode(RenderTarget);

            ClearBackground(BLACK);

            BeginMode2D(SimTag->ActiveCamera->Camera);

            ObjDraw(*Sim);
            
            EndMode2D();

        EndTextureMode();

        BeginDrawing();

            BeginShaderMode(BloomShader);
                DrawTextureRec(RenderTarget.texture, (Rectangle){ 0, 0, (float)RenderTarget.texture.width, (float)-RenderTarget.texture.height }, (Vector2){ 0, 0 }, WHITE);
            EndShaderMode();
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}

// handle all file loading based on folder/file and extensions
void LoadDataFolder() {

    double StartTime = GetTime(); // track load time
    
    sh_new_arena(TomCtx.AnimMap);
    sh_new_arena(TomCtx.TextureMap); // stores key pointers with arenas
    for (const auto& Entry : std::filesystem::directory_iterator("data")) {
        std::string FilePath = Entry.path().string();
        std::string FileExt = Entry.path().extension().string();
        std::string FileNameStr = Entry.path().stem().string();
        const char* FileName = FileNameStr.c_str(); // not sure why but I have to make FileNameStr first

        if (std::filesystem::is_regular_file(Entry.path())) {

            printf("file_path: %s, file_ext: %s, file_stem: %s\n", FilePath.c_str(), FileExt.c_str(), FileName);
            if (FileExt == ".png") {
                shput(TomCtx.TextureMap, FileName, LoadTexture(FilePath.c_str()));
                printf("added file: %s to TextureMap\n\n\n\n\n", FileName);
            }
        }
        if (std::filesystem::is_directory(Entry.path())) {
            
            size_t FoundPos = FileNameStr.find("_anim");
            if (FoundPos != std::string::npos) {
                auto AnimIterator = std::filesystem::directory_iterator(FilePath);
                size_t FileCount = std::distance(AnimIterator, std::filesystem::directory_iterator{});
                tray<Texture2D>* Frames = MakeTray<Texture2D>(FileCount);

                for (const auto& AnimEntry :  std::filesystem::directory_iterator(FilePath)) {
                    std::string FramePath = AnimEntry.path().string();

                    printf("filename: %s, framepath: %s\n\n", FileName, FramePath.c_str());
                    TrayAdd(Frames, LoadTexture(FramePath.c_str()));
                }
                shput(TomCtx.AnimMap, FileName, Frames);
            }
        }
    }

    double LoadDuration = GetTime() - StartTime;
    printf("\n\n\n Textures loaded in %f seconds\n\n\n", LoadDuration);
}