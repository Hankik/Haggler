#include "tom.h"
#include "allocators.h"
#include "stdio.h"

#pragma once

template <typename T>
struct tray
{
    T *EmptyPtr = nullptr;
    T *Ptr = nullptr;
    int Amt = 0;
    int Cap = 0;

    // Index operator overload (non-const)
    T &operator[](int Index)
    {
        return Ptr[Index];
    }

    // Index operator overload (const)
    const T &operator[](int Index) const
    {
        return Ptr[Index];    
    }
};

template <typename T>
tray<T>* MakeTray(unsigned int Capacity)
{
    tray<T>* NewTray = (tray<T>*) BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(tray<T>));
    if (NewTray) {
        NewTray->Ptr = (T *)BuddyAllocatorAlloc(TomCtx.BuddyAlloc, sizeof(T) * Capacity);
        NewTray->Cap = NewTray->Ptr ? Capacity : 0;
        NewTray->Amt = 0;
        return NewTray;
    }
    
    return nullptr;
}

template <typename T>
bool TrayAdd(tray<T> * Tray, T Item) {
    if (Tray->Amt < Tray->Cap) {
        (*Tray)[Tray->Amt] = Item;
        Tray->Amt++;
        return true;
    } else {
        return false;
    }
    
}