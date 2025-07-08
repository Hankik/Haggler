#include "context.h"
#include "button.h"
#include "tray.h"
#include "raymath.h"

obj *MakeButtonObj() {
    obj *Button = MakeObj();
    Button->Children = MakeTray<obj *>(0);
    Button->Tags = MakeTray<tag *>(1);
    tag* TagsToAdd[] = {
        MakeButtonTag(),
    };
    TryAddTags(*Button, ArrayToTray(TagsToAdd));

    return Button;
};

tag* MakeButtonTag() {
    button_tag* ButtonTag = (button_tag*) MakeAlloc<button_tag>();
    *ButtonTag = button_tag{};
    ButtonTag->TickFn = ButtonTagTick;
    ButtonTag->DrawFn = ButtonTagDraw;
    ButtonTag->OnGetMsgFn = OnButtonGetMsg;
    ButtonTag->Type = BUTTON;
    ButtonTag->Size = (Vector2){80, 40};
    
    return ButtonTag;
}

void ButtonTagTick(tag &Tag) {
    button_tag& ButtonTag = (button_tag&) Tag;
    Vector2 Position = ButtonTag.Obj->LocalPos;
    Vector2 Size = ButtonTag.Size;
    Vector2 MousePos = (ButtonTag.IsHudElement) ? GetMousePosition() : TomCtx.SimTag->ActiveCamera->Mouse;
    Rectangle Bounds {Position.x - Size.x*0.5f,Position.y - Size.y*0.5f, Size.x, Size.y};
    switch (ButtonTag.ButtonState) {
        case IDLE: {
            if (CheckCollisionPointRec(MousePos, Bounds)) {

                ButtonTag.ButtonState = HOVERED;
            }

        } break;
        case HOVERED: {

            if (!CheckCollisionPointRec(MousePos, Bounds)) {

                ButtonTag.ButtonState = IDLE;
            } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                ButtonTag.ButtonState = PRESSED;
            } 

        } break;
        case PRESSED: {
            if (!CheckCollisionPointRec(MousePos, Bounds)) {

                ButtonTag.ButtonState = IDLE;
            } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                printf("Button Pressed\n");
                // do something
                ButtonTag.ButtonState = HOVERED;
            } 
        } break;
    }
}

void ButtonTagDraw(const tag &Tag) {
    button_tag& ButtonTag = (button_tag&) Tag;

    float Saturation = 1;
    float Value = 1;

    switch (ButtonTag.ButtonState) {
        case IDLE: {

        } break;
        case HOVERED: {

            Value -= 0.25;

        } break;
        case PRESSED: {

            Value -= 0.5;
            
        } break;
    }
    Vector2 Position = Tag.Obj->LocalPos;
    Vector2 Size = ButtonTag.Size;
    Rectangle Shape {Position.x - Size.x*0.5f,Position.y-Size.y*0.5f, Size.x, Size.y};
    Color Fill = ColorAlpha(ColorFromHSV(fmod(GetTime(), 360), Saturation, Value), 0.5);

    DrawRectangleRounded(Shape,
        0.3, // rounding amt
        8,   // segment amt
        Fill
    );
    DrawRectangleRoundedLinesEx(Shape,
        0.3, // rounding amt
        8,   // segment amt
        2,
        WHITE
    );
}

bool OnButtonGetMsg(tag &Tag, msg &Msg) {

    return false;
}