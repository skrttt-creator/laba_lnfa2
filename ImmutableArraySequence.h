#pragma once
#include "Sequence.h"
#include "DynamicArray.h"
#include "Exceptions.h"
#include "UniquePtr.h"

template <class T>
class ImmutableArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* array;

    explicit ImmutableArraySequence(DynamicArray<T>* arr)
        : array(arr) {
    }

public:
    ImmutableArraySequence()
        : array(new DynamicArray<T>(0)) {
    }

    ImmutableArraySequence(const T* items, int count)
        : array(new DynamicArray<T>(items, count)) {
    }

    ImmutableArraySequence(const ImmutableArraySequence<T>& other)
        : array(new DynamicArray<T>(*other.array)) {
    }

    ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& other) {
        if (this != &other) {
            delete array;
            array = new DynamicArray<T>(*other.array);
        }
        return *this;
    }

    ~ImmutableArraySequence() override {
        delete array;
    }

    T GetFirst() const override {
        if (array->GetSize() == 0) {
            throw IndexOutOfRange("Последовательность пуста");
        }
        return array->Get(0);
    }

    T GetLast() const override {
        int size = array->GetSize();
        if (size == 0) {
            throw IndexOutOfRange("Последовательность пуста");
        }
        return array->Get(size - 1);
    }

    T Get(int index) const override {
        return array->Get(index);
    }

    int GetLength() const override {
        return array->GetSize();
    }

    T operator[](int index) const override {
        return (*array)[index];
    }

    T& operator[](int index) override {
        throw IndexOutOfRange("Невозможно изменить элемент в неизменяемой последовательности");
    }

    Sequence<T>* Append(const T& item) override {
        int oldSize = array->GetSize();
        DynamicArray<T>* newArray = new DynamicArray<T>(oldSize + 1);

        for (int i = 0; i < oldSize; ++i) {
            newArray->Set(i, array->Get(i));
        }
        newArray->Set(oldSize, item);

        return new ImmutableArraySequence<T>(newArray);
    }

    Sequence<T>* Prepend(const T& item) override {
        int oldSize = array->GetSize();
        DynamicArray<T>* newArray = new DynamicArray<T>(oldSize + 1);

        for (int i = 0; i < oldSize; ++i) {
            newArray->Set(i + 1, array->Get(i));
        }
        newArray->Set(0, item);

        return new ImmutableArraySequence<T>(newArray);
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        int size = array->GetSize();
        if (index < 0 || index > size) {
            throw IndexOutOfRange("Индекс вне диапазона");
        }

        DynamicArray<T>* newArray = new DynamicArray<T>(size + 1);

        for (int i = 0; i < index; ++i) {
            newArray->Set(i, array->Get(i));
        }

        newArray->Set(index, item);

        for (int i = index; i < size; ++i) {
            newArray->Set(i + 1, array->Get(i));
        }

        return new ImmutableArraySequence<T>(newArray);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        return new ImmutableArraySequence<T>(array->GetSubArray(startIndex, endIndex));
    }

    Sequence<T>* Concat(Sequence<T>* list) const override {
        int currentSize = array->GetSize();
        int listSize = list->GetLength();
        DynamicArray<T>* newArray = new DynamicArray<T>(currentSize + listSize);

        for (int i = 0; i < currentSize; ++i) {
            newArray->Set(i, array->Get(i));
        }

        UniquePtr<Iterator<T>> it(list->GetIterator());
        int insertIndex = currentSize;
        while (it->HasNext()) {
            newArray->Set(insertIndex++, it->Next());
        }

        return new ImmutableArraySequence<T>(newArray);
    }

    Iterator<T>* GetIterator() const override {
        return new typename DynamicArray<T>::ArrayIterator(array);
    }
};