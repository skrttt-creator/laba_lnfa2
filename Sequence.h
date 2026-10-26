#pragma once
#include "Exceptions.h"
#include "ICollection.h"
#include "IEnumerable.h"
#include <cstddef>

template <class T>
class Sequence : public ICollection<T>, public IEnumerable<T> {
protected:
    virtual Sequence<T>* Instance() = 0;
    virtual Sequence<T>* Clone() const = 0;
    virtual Sequence<T>* EmptyClone() const = 0;

    virtual void AppendInternal(const T& item) = 0;
    virtual void PrependInternal(const T& item) = 0;
    virtual void InsertAtInternal(const T& item, size_t index) = 0;

public:
    virtual ~Sequence() = default;

    virtual size_t GetCount() const override = 0;
    virtual T Get(size_t index) const override = 0;

    size_t GetLength() const {
        return this->GetCount();
    }

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;

    virtual T operator[](size_t index) const = 0;
    virtual T& operator[](size_t index) = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, size_t index) = 0;
    virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* other) const = 0;

    Sequence<T>* Map(T (*func)(T)) const;
    Sequence<T>* Where(bool (*func)(T)) const;
    T Reduce(T (*func)(T, T), T initial) const;
    Sequence<T>* Map(T (*func)(T, size_t)) const;
    Sequence<T>* Zip(const Sequence<T>* other, T (*combiner)(T, T)) const;

    virtual IEnumerator<T>* GetEnumerator() const override = 0;
};

#include "Sequence.tpp"