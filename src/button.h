#include "tom.h"

obj* MakeButton();

enum button_state {
    IDLE,
    HOVERED,
    PRESSED
};

struct button_tag : tag {
    Vector2 Offset;
    Vector2 Size;
    button_state ButtonState = button_state::IDLE;
};

