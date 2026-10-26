#pragma once
#include "Sequence.h"

template <class T>
Sequence<T>* Sequence<T>::Map(T (*func)(T)) const {
    if (func == nullptr) {
        throw InvalidArgument("Map: function is null");
    }
    Sequence<T>* result = EmptyClone();
    IEnumerator<T>* it = this->GetEnumerator();
    while (it->MoveNext()) {
        result->AppendInternal(func(it->GetCurrent()));
    }
    delete it;
    return result;
}

template <class T>
Sequence<T>* Sequence<T>::Where(bool (*func)(T)) const {
    if (func == nullptr) {
        throw InvalidArgument("Where: function is null");
    }
    Sequence<T>* result = EmptyClone();
    IEnumerator<T>* it = this->GetEnumerator();
    while (it->MoveNext()) {
        T current = it->GetCurrent();
        if (func(current)) {
            result->AppendInternal(current);
        }
    }
    delete it;
    return result;
}

template <class T>
T Sequence<T>::Reduce(T (*func)(T, T), T initial) const {
    if (func == nullptr) {
        throw InvalidArgument("Reduce: function is null");
    }
    T result = initial;
    IEnumerator<T>* it = this->GetEnumerator();
    while (it->MoveNext()) {
        result = func(result, it->GetCurrent());
    }
    delete it;
    return result;
}

template <class T>
Sequence<T>* Sequence<T>::Map(T (*func)(T, size_t)) const {
    if (func == nullptr) {
        throw InvalidArgument("Map: function is null");
    }
    Sequence<T>* result = this->EmptyClone();
    IEnumerator<T>* it = this->GetEnumerator();
    size_t index = 0;
    while (it->MoveNext()) {
        result->AppendInternal(func(it->GetCurrent(), index));
        index++;
    }
    delete it;
    return result;
}

template <class T>
Sequence<T>* Sequence<T>::Zip(const Sequence<T>* other, T (*combiner)(T, T)) const {
    if (other == nullptr || combiner == nullptr) {
        throw InvalidArgument("Zip: other sequence or combiner is null");
    }
    Sequence<T>* result = this->EmptyClone();
    IEnumerator<T>* it1 = this->GetEnumerator();
    IEnumerator<T>* it2 = other->GetEnumerator();
    while (it1->MoveNext() && it2->MoveNext()) {
        result->AppendInternal(combiner(it1->GetCurrent(), it2->GetCurrent()));
    }
    delete it1;
    delete it2;
    return result;
}