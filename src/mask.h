#pragma once

#include "tom.h"
#include "context.h"

template <typename T>
struct mask : T {
    T* Mask = nullptr;
    T* Original = nullptr;
};

template <typename T>
mask<T>* MakeMask(T* Original) {
    mask<T>* Mask = (mask<T>*) MakeAlloc<mask<T>>();
    Mask->Mask = (T*) MakeAlloc<T>();
    *static_cast<T*>(Mask->Mask) = *Original; // Use assignment for safety
    Mask->Original = Original;
    return Mask;
}