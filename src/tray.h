#include "tom.h"
#include "allocators.h"


#pragma once

template <typename T>
struct tray
{
    T *Ptr = nullptr;
    int Amt = 0;
    int Cap = 0;

    // Index operator overload (non-const)
    T &operator[](int index)
    {
        return Ptr[index];
    }

    // Index operator overload (const)
    const T &operator[](int index) const
    {
        return Ptr[index];
    }
};

template <typename T>
tray<T> operator+(const tray<T> &a, const tray<T> &b)
{
    // tray<T> result =
}

template <typename T>
tray<T> MakeTray(Arena *a, unsigned int Capacity)
{
    tray<T> t;
    t.Ptr = (T *)ArenaAlloc(a, Capacity);
    t.Cap = Capacity;
    t.Amt = 0;
    return t;
}

template <typename T>
int ReserveTrayHandle(tray<T> *Tray)
{
    if (Tray->Amt < Tray->Cap)
    {
        int Handle = Tray->Amt;
        Tray->Amt++;
        return Handle;
    }
    return -1;
}