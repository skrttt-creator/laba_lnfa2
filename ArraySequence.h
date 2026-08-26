#pragma once
#include "Sequence.h"
#include "DynamicArray.h"
#include "Exceptions.h"
#include "UniquePtr.h"

template <class T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* items;

    explicit ArraySequence(DynamicArray<T>* arr)
        : items(arr) {
    }

public:
    ArraySequence()
        : items(new DynamicArray<T>(0)) {
    }

    ArraySequence(const T* data, int count)
        : items(new DynamicArray<T>(data, count)) {
    }

    ArraySequence(const ArraySequence<T>& other)
        : items(new DynamicArray<T>(*other.items)) {
    }

    ArraySequence<T>& operator=(const ArraySequence<T>& other) {
        if (this != &other) {
            delete items;
            items = new DynamicArray<T>(*other.items);
        }
        return *this;
    }

    ~ArraySequence() override {
        delete items;
    }

    T GetFirst() const override {
        if (items->GetSize() == 0) {
            throw IndexOutOfRange("Последовательность пуста");
        }
        return items->Get(0);
    }

    T GetLast() const override {
        if (items->GetSize() == 0) {
            throw IndexOutOfRange("Последовательность пуста");
        }
        return items->Get(items->GetSize() - 1);
    }

    T Get(int index) const override {
        return items->Get(index);
    }

    int GetLength() const override {
        return items->GetSize();
    }

    T operator[](int index) const override {
        return (*items)[index];
    }

    T& operator[](int index) override {
        return (*items)[index];
    }

    Sequence<T>* Append(const T& val) override {
        items->Resize(items->GetSize() + 1);
        items->Set(items->GetSize() - 1, val);
        return this;
    }

    Sequence<T>* Prepend(const T& val) override {
        int sz = items->GetSize();
        items->Resize(sz + 1);

        for (int i = sz; i > 0; i--) {
            items->Set(i, items->Get(i - 1));
        }

        items->Set(0, val);
        return this;
    }

    Sequence<T>* InsertAt(const T& val, int index) override {
        int sz = items->GetSize();
        if (index < 0 || index > sz) {
            throw IndexOutOfRange("Индекс вне диапазона");
        }

        items->Resize(sz + 1);

        for (int i = sz; i > index; i--) {
            items->Set(i, items->Get(i - 1));
        }

        items->Set(index, val);
        return this;
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        return new ArraySequence<T>(items->GetSubArray(startIndex, endIndex));
    }

    Sequence<T>* Concat(Sequence<T>* list) const override {
        ArraySequence<T>* result = new ArraySequence<T>();

        for (int i = 0; i < this->GetLength(); ++i) {
            result->Append(this->Get(i));
        }

        UniquePtr<Iterator<T>> it(list->GetIterator());
        while (it->HasNext()) {
            result->Append(it->Next());
        }

        return result;
    }

    Iterator<T>* GetIterator() const override {
        return new typename DynamicArray<T>::ArrayIterator(items);
    }
};