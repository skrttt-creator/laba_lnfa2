#pragma once

#include "Sequence.h"
#include "LinkedList.h"
#include "Exceptions.h"
#include <cstddef>

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> list;

    virtual Sequence<T>* Clone() const = 0;
    virtual Sequence<T>* Instance() = 0;

    Sequence<T>* EmptyClone() const override = 0;

    void AppendInternal(const T& item) override;
    void PrependInternal(const T& item) override;
    void InsertAtInternal(const T& item, size_t index) override;

public:
    ListSequence();
    ListSequence(const T* items, size_t count);
    explicit ListSequence(const LinkedList<T>& lst);
    ListSequence(const ListSequence<T>& other);
    virtual ~ListSequence() = default;

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
class MutableListSequence : public ListSequence<T> {
protected:
    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
    
    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* EmptyClone() const override {
        return new MutableListSequence<T>();
    }

public:
    using ListSequence<T>::ListSequence;
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
    
    Sequence<T>* Instance() override {
        return this->Clone();
    }

    Sequence<T>* EmptyClone() const override {
        return new ImmutableListSequence<T>();
    }

public:
    using ListSequence<T>::ListSequence;
};

#include "ListSequence.tpp"