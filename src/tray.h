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
tray<T>* operator+(const tray<T> &A, const tray<T> &B)
{
    tray<T>* Result = MakeTray<T>(&Globals::BuddyAlloc, A->Amt + B->Amt);
    for (int Index = 0; Index < A->Amt; ++Index) {
        Result[Index] = A[Index];
    }
    for (int Index = 0; Index < B->Amt; ++Index) {
        Result[A->Amt + Index] = B[Index];
    }
    BuddyAllocatorFree(&Globals::BuddyAlloc, A);
    BuddyAllocatorFree(&Globals::BuddyAlloc, B);
    return Result;
}

template <typename T>
tray<T> MakeTray(Buddy_Allocator *allocator, unsigned int Capacity)
{
    tray<T> t;
    t.Ptr = (T *)BuddyAllocatorAlloc(allocator, Capacity);
    t.Cap = Capacity;
    t.Amt = 0;
    return t;
}