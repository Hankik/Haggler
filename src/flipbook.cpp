#include "flipbook.h"
#include "tray.h"
#include "context.h"

obj* MakeFlipbookObj() {
    obj* Flipbook = MakeObj();
    Flipbook->Children = MakeTray<obj*>(0);
    tag* TagsToAdd[] = {
        MakeFlipbookTag(),
    };
    AddTags(*Flipbook, ArrayToTray(TagsToAdd));
    return Flipbook;
}

tag* MakeFlipbookTag() {
    flipbook_tag* FlipbookTag = (flipbook_tag*) MakeAlloc<flipbook_tag>();
    *FlipbookTag = flipbook_tag{};
    FlipbookTag->Type = FLIPBOOK;
    FlipbookTag->DrawFn = FlipbookTagDraw;
    FlipbookTag->TickFn = FlipbookTagTick;
    FlipbookTag->OnGetMsgFn = OnFlipbookGetMsg;
    return FlipbookTag;
}

void FlipbookTagTick(tag& Tag) {
    flipbook_tag& FlipbookTag = (flipbook_tag&) Tag;
    if (FlipbookTag.Frames) {
        FlipbookTag.NextFrameTimer += GetFrameTime();
        if (FlipbookTag.NextFrameTimer >= FlipbookTag.NextFrameDelay) {
            FlipbookTag.CurrentFrame = (FlipbookTag.CurrentFrame + 1) % FlipbookTag.Frames->Amt; 
            FlipbookTag.NextFrameTimer = 0;
        }
    }
}

void FlipbookTagDraw(const tag& Tag) {
    flipbook_tag& FlipbookTag = (flipbook_tag&) Tag;
    if (FlipbookTag.Frames) {
        Vector2 Position = GetGlobalPos(*GetObj(FlipbookTag));
        tray<Texture2D>& Frames = *FlipbookTag.Frames;
        Texture2D& FrameToDraw = Frames[FlipbookTag.CurrentFrame];
        Vector2 FrameSize {(float)FrameToDraw.width, (float)FrameToDraw.height};
        
        DrawTexture(FrameToDraw, 
            Position.x - FrameSize.x*0.5, 
            Position.y - FrameSize.y*0.5, 
            WHITE
        );        
    }
}
bool OnFlipbookGetMsg(tag& Tag, msg& Msg) {

    return false;
    
}
