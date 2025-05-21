#pragma once

struct mail
{
    virtual ~mail() = default;
};
struct key_pressed_mail : mail
{
    char Key = 0;
    key_pressed_mail(char Key) : Key(Key) {}
};
