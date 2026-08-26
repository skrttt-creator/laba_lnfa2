#pragma once
#include "Exceptions.h"
#include "Iterator.h"

template <class T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

public:
    class ArrayIterator : public Iterator<T> {
    private:
        const DynamicArray<T>* arr;
        int current;

    public:
        explicit ArrayIterator(const DynamicArray<T>* a)
            : arr(a), current(0) {
        }

        bool HasNext() const override {
            return current < arr->GetSize();
        }

        const T& Next() override {
            return (*arr)[current++];
        }

        void Reset() override {
            current = 0;
        }
    };

    explicit DynamicArray(int size)
        : size(size), capacity(size > 0 ? size : 1) {
        if (size < 0) {
            throw InvalidArgument("Отрицательный размер");
        }
        data = new T[capacity];
    }

    DynamicArray(const T* items, int count)
        : size(count), capacity(count > 0 ? count : 1) {
        if (count < 0) {
            throw InvalidArgument("Отрицательное количество элементов");
        }
        data = new T[capacity];
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    DynamicArray(const DynamicArray<T>& other)
        : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (int i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    DynamicArray<T>* GetSubArray(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= size || startIndex > endIndex) {
            throw IndexOutOfRange("Ошибка границ подмассива");
        }

        int newSize = endIndex - startIndex + 1;
        DynamicArray<T>* subArray = new DynamicArray<T>(newSize);

        for (int i = 0; i < newSize; ++i) {
            subArray->Set(i, this->Get(startIndex + i));
        }

        return subArray;
    }

    ~DynamicArray() {
        delete[] data;
    }

    T Get(int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("Ошибка индекса массива");
        }
        return data[index];
    }

    int GetSize() const {
        return size;
    }

    void Set(int index, const T& value) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("Ошибка индекса массива");
        }
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw InvalidArgument("Отрицательный размер");
        }

        if (newSize > capacity) {
            int newCap = newSize * 2;
            T* newData = new T[newCap];

            for (int i = 0; i < size; ++i) {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
            capacity = newCap;
        }

        size = newSize;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("Ошибка индекса массива");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("Ошибка индекса массива");
        }
        return data[index];
    }
};