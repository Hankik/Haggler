#pragma once

#include "tom.h"

enum msg_type
{
    KEY_PRESS_MSG,
    KEY_RELEASE_MSG,
};

struct msg
{
    msg_type Type;
};

struct key_press_msg : msg {
    int Key = -1;
};

struct key_release_msg : msg {
    int Key = -1;
};