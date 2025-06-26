#include "tom.h"

struct key_press_msg : msg {
    msg_type Type = msg_type::KEY_PRESS_MSG;
    char Key = NULL;
};