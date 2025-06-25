#include <array>
#include "tom.h"
extern "C"
{
#include "allocators.h"
}
#include <cstring>

#pragma once

namespace Globals
{

    template <typename T, typename Callback>
    T Create(Callback &&callback)
    {
        T value{};       // Default-construct the object
        callback(value); // Let the callback initialize or mutate it
        return value;
    }
    const int ALLOCATOR_SIZE = 1024000;
    unsigned char BackingBuffer[ALLOCATOR_SIZE];
    Buddy_Allocator BuddyAlloc;

    // template <typename T>
    // int ReserveHandle()
    // {

    //     constexpr int Alignment = alignof(T);
    //     int AlignedHandle = (CurrentHandle + Alignment - 1) & ~(Alignment - 1);
    //     int ReservationSize = sizeof(T);
    //     if (CurrentHandle + ReservationSize <= MEM_SIZE)
    //     {
    //         CurrentHandle = AlignedHandle + ReservationSize;
    //         return AlignedHandle;
    //     }
    //     else
    //     {
    //         return -1;
    //         // not enough space
    //     }
    // }

}
