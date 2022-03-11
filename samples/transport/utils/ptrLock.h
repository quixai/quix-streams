#pragma once

template<typename T>
class PtrLock
{
    T* ptr_;
public:
    PtrLock(T* ptr) : ptr_(ptr) { }
    ~PtrLock() {
        delete ptr_;
    }
};
