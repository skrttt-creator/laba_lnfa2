#pragma once

template <class T>
class UniquePtr {
private:
    T* ptr;
public:
    explicit UniquePtr(T* p = nullptr) :
    ptr(p) {}
    ~UniquePtr() { delete ptr; }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    T* Get() const { return ptr; }
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }

    void Reset(T* p = nullptr) {
        if (ptr != p) {
            delete ptr;
        }
        ptr = p;
    }
    T* Release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

