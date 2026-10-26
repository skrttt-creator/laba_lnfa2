#pragma once

#include "ListSequence.h"

template <class T>
ListSequence<T>::ListSequence() : list() {}

template <class T>
ListSequence<T>::ListSequence(const T* items, size_t count) : list(items, count) {}

template <class T>
ListSequence<T>::ListSequence(const LinkedList<T>& lst) : list(lst) {}

template <class T>
ListSequence<T>::ListSequence(const ListSequence<T>& other) : list(other.list) {}

template <class T>
void ListSequence<T>::AppendInternal(const T& item) {
    list.Append(item);
}

template <class T>
void ListSequence<T>::PrependInternal(const T& item) {
    list.Prepend(item);
}

template <class T>
void ListSequence<T>::InsertAtInternal(const T& item, size_t index) {
    list.InsertAt(item, index);
}

template <class T>
size_t ListSequence<T>::GetCount() const {
    return list.GetLength();
}

template <class T>
T ListSequence<T>::Get(size_t index) const {
    return list.Get(index);
}

template <class T>
T ListSequence<T>::GetFirst() const {
    return list.GetFirst();
}

template <class T>
T ListSequence<T>::GetLast() const {
    return list.GetLast();
}

template <class T>
T ListSequence<T>::operator[](size_t index) const {
    return list.Get(index);
}

template <class T>
T& ListSequence<T>::operator[](size_t index) {
    return list.GetRef(index);
}

template <class T>
Sequence<T>* ListSequence<T>::Append(const T& item) {
    ListSequence<T>* inst = static_cast<ListSequence<T>*>(this->Instance());
    inst->AppendInternal(item);
    return inst;
}

template <class T>
Sequence<T>* ListSequence<T>::Prepend(const T& item) {
    ListSequence<T>* inst = static_cast<ListSequence<T>*>(this->Instance());
    inst->PrependInternal(item);
    return inst;
}

template <class T>
Sequence<T>* ListSequence<T>::InsertAt(const T& item, size_t index) {
    ListSequence<T>* inst = static_cast<ListSequence<T>*>(this->Instance());
    inst->InsertAtInternal(item, index);
    return inst;
}

template <class T>
Sequence<T>* ListSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex >= GetCount() || endIndex >= GetCount() || startIndex > endIndex) {
        throw IndexOutOfRange("GetSubsequence: Invalid bounds");
    }
    
    ListSequence<T>* result = static_cast<ListSequence<T>*>(this->EmptyClone());
    
    for (size_t i = startIndex; i <= endIndex; ++i) {
        result->AppendInternal(this->Get(i));
    }
    
    return result;
}

template <class T>
Sequence<T>* ListSequence<T>::Concat(const Sequence<T>* other) const {
    if (other == nullptr) {
        throw InvalidArgument("Concat: other sequence is null");
    }

    ListSequence<T>* result = static_cast<ListSequence<T>*>(this->Clone());
    
    IEnumerator<T>* it = other->GetEnumerator();
    while (it->MoveNext()) {
        result->AppendInternal(it->GetCurrent());
    }
    delete it;
    
    return result;
}

template <class T>
IEnumerator<T>* ListSequence<T>::GetEnumerator() const {
    return list.GetEnumerator();
}