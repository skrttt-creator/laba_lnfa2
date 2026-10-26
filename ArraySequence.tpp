#pragma once
#include "ArraySequence.h"

template <class T>
ArraySequence<T>::ArraySequence() : array(0) {}

template <class T>
ArraySequence<T>::ArraySequence(const T* items, size_t count) : array(items, count) {}

template <class T>
ArraySequence<T>::ArraySequence(const DynamicArray<T>& arr) : array(arr) {}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other) : array(other.array) {}

template <class T>
void ArraySequence<T>::AppendInternal(const T& item) {
    size_t currentSize = array.GetSize();
    array.Resize(currentSize + 1);
    array.Set(currentSize, item);
}

template <class T>
void ArraySequence<T>::PrependInternal(const T& item) {
    size_t currentSize = array.GetSize();
    array.Resize(currentSize + 1);
    for (size_t i = currentSize; i > 0; --i) {
        array.Set(i, array.Get(i - 1));
    }
    array.Set(0, item);
}

template <class T>
void ArraySequence<T>::InsertAtInternal(const T& item, size_t index) {
    size_t currentSize = array.GetSize();
    if (index > currentSize) {
        throw IndexOutOfRange("IndexOutOfRange");
    }
    array.Resize(currentSize + 1);
    for (size_t i = currentSize; i > index; --i) {
        array.Set(i, array.Get(i - 1));
    }
    array.Set(index, item);
}

template <class T>
size_t ArraySequence<T>::GetCount() const {
    return array.GetSize();
}

template <class T>
T ArraySequence<T>::Get(size_t index) const {
    return array.Get(index);
}

template <class T>
T ArraySequence<T>::GetFirst() const {
    if (GetCount() == 0) {
        throw IndexOutOfRange("IndexOutOfRange");
    }
    return array.Get(0);
}

template <class T>
T ArraySequence<T>::GetLast() const {
    if (GetCount() == 0) {
        throw IndexOutOfRange("IndexOutOfRange");
    }
    return array.Get(GetCount() - 1);
}

template <class T>
T ArraySequence<T>::operator[](size_t index) const {
    return array.Get(index);
}

template <class T>
T& ArraySequence<T>::operator[](size_t index) {
    return array.GetRef(index);
}

template <class T>
Sequence<T>* ArraySequence<T>::Append(const T& item) {
    ArraySequence<T>* inst = static_cast<ArraySequence<T>*>(this->Instance());
    inst->AppendInternal(item);
    return inst;
}

template <class T>
Sequence<T>* ArraySequence<T>::Prepend(const T& item) {
    ArraySequence<T>* inst = static_cast<ArraySequence<T>*>(this->Instance());
    inst->PrependInternal(item);
    return inst;
}

template <class T>
Sequence<T>* ArraySequence<T>::InsertAt(const T& item, size_t index) {
    ArraySequence<T>* inst = static_cast<ArraySequence<T>*>(this->Instance());
    inst->InsertAtInternal(item, index);
    return inst;
}

template <class T>
Sequence<T>* ArraySequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex >= GetCount() || endIndex >= GetCount() || startIndex > endIndex) {
        throw IndexOutOfRange("GetSubsequence: Invalid bounds");
    }
    
    ArraySequence<T>* result = static_cast<ArraySequence<T>*>(this->EmptyClone());
    
    for (size_t i = startIndex; i <= endIndex; ++i) {
        result->AppendInternal(this->Get(i));
    }
    
    return result;
}

template <class T>
Sequence<T>* ArraySequence<T>::Concat(const Sequence<T>* other) const {
    if (other == nullptr) {
        throw InvalidArgument("Concat: other sequence is null"); 
    }

    ArraySequence<T>* result = static_cast<ArraySequence<T>*>(this->Clone());
    
    IEnumerator<T>* it = other->GetEnumerator();
    while (it->MoveNext()) {
        result->AppendInternal(it->GetCurrent());
    }
    delete it;
    
    return result;
}

template <class T>
IEnumerator<T>* ArraySequence<T>::GetEnumerator() const {
    return array.GetEnumerator();
}