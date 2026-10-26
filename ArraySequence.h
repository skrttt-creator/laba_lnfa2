#pragma once

#include "Sequence.h"
#include "DynamicArray.h"
#include "Exceptions.h"
#include <cstddef>

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> array;

    virtual Sequence<T>* Clone() const = 0;
    virtual Sequence<T>* Instance() = 0;

    Sequence<T>* EmptyClone() const override = 0;

    void AppendInternal(const T& item) override;
    void PrependInternal(const T& item) override;
    void InsertAtInternal(const T& item, size_t index) override;

public:
    ArraySequence();
    ArraySequence(const T* items, size_t count);
    explicit ArraySequence(const DynamicArray<T>& arr);
    ArraySequence(const ArraySequence<T>& other);
    virtual ~ArraySequence() = default;

    size_t GetCount() const override;
    T Get(size_t index) const override;
    T GetFirst() const override;
    T GetLast() const override;

    T operator[](size_t index) const override;
    T& operator[](size_t index) override;

    Sequence<T>* Append(const T& item) override;
    Sequence<T>* Prepend(const T& item) override;
    Sequence<T>* InsertAt(const T& item, size_t index) override;

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override;
    Sequence<T>* Concat(const Sequence<T>* other) const override;

    IEnumerator<T>* GetEnumerator() const override;
};

template <class T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    Sequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
    
    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* EmptyClone() const override {
        return new MutableArraySequence<T>();
    }

public:
    using ArraySequence<T>::ArraySequence;
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
    
    Sequence<T>* Instance() override {
        return this->Clone();
    }

    Sequence<T>* EmptyClone() const override {
        return new ImmutableArraySequence<T>();
    }

public:
    using ArraySequence<T>::ArraySequence;
};

#include "ArraySequence.tpp"