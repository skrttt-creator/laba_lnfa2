#pragma once
#include "ArraySequence.h"
#include "Exceptions.h"
#include "UniquePtr.h"

struct Stats {
    int min;
    int max;
    int sum;
    int count;
    double avg;
};

template <class T1, class T2>
struct Tuple {
    T1 first;
    T2 second;

    Tuple() = default;

    Tuple(const T1& f, const T2& s)
        : first(f), second(s) {
    }
};

Stats StatsReducer(const Stats& acc, const int& val);
Stats GetSequenceStats(const ArraySequence<int>& seq);
double GetMedian(const ArraySequence<int>& seq);
int MirrorSumMapper(const int& val, int index, const Sequence<int>* seq);
ArraySequence<int>* GetMirrorSums(const ArraySequence<int>& seq);


template <class T>
Sequence<Sequence<T>*>* Split(const Sequence<T>& seq, bool (*condition)(const T&)) {
    ArraySequence<Sequence<T>*>* result = new ArraySequence<Sequence<T>*>();
    ArraySequence<T>* currentFragment = new ArraySequence<T>();

    UniquePtr<Iterator<T>> it(seq.GetIterator());

    while (it->HasNext()) {
        const T& item = it->Next();

        if (condition(item)) {
            result->Append(currentFragment);
            currentFragment = new ArraySequence<T>();
        } else {
            currentFragment->Append(item);
        }
    }

    result->Append(currentFragment);
    return result;
}

template <class T1, class T2>
ArraySequence<Tuple<T1, T2>>* ZipSequences(const Sequence<T1>& seq1, const Sequence<T2>& seq2) {
    ArraySequence<Tuple<T1, T2>>* result = new ArraySequence<Tuple<T1, T2>>();

    UniquePtr<Iterator<T1>> it1(seq1.GetIterator());
    UniquePtr<Iterator<T2>> it2(seq2.GetIterator());

    while (it1->HasNext() && it2->HasNext()) {
        result->Append(Tuple<T1, T2>(it1->Next(), it2->Next()));
    }

    return result;
}

template <class T>
Sequence<T>* Slice(Sequence<T>* seq, int index, int count, Sequence<T>* items) {
    int len = seq->GetLength();

    if (index < 0) {
        index = len + index;
    }

    if (index < 0 || index > len) {
        throw IndexOutOfRange("Срез: индекс вне диапазона");
    }

    ArraySequence<T>* result = new ArraySequence<T>();
    UniquePtr<Iterator<T>> it(seq->GetIterator());
    int currentIndex = 0;

    while (it->HasNext() && currentIndex < index) {
        result->Append(it->Next());
        currentIndex++;
    }

    if (items != nullptr) {
        UniquePtr<Iterator<T>> itemsIt(items->GetIterator());
        while (itemsIt->HasNext()) {
            result->Append(itemsIt->Next());
        }
    }

    while (it->HasNext() && currentIndex < index + count) {
        it->Next();
        currentIndex++;
    }

    while (it->HasNext()) {
        result->Append(it->Next());
    }

    return result;
}

//Классический Map
template <class T, class T2>
Sequence<T2>* Map(const Sequence<T>* seq, T2 (*mapper)(const T&)) {
    ArraySequence<T2>* result = new ArraySequence<T2>();
    UniquePtr<Iterator<T>> it(seq->GetIterator());

    while (it->HasNext()) {
        result->Append(mapper(it->Next()));
    }

    return result;
}

//Map с контекстом
template <class T, class T2>
Sequence<T2>* MapContext(const Sequence<T>* seq, T2 (*mapper)(const T&, int, const Sequence<T>*)) {
    ArraySequence<T2>* result = new ArraySequence<T2>();
    UniquePtr<Iterator<T>> it(seq->GetIterator());

    int index = 0;
    while (it->HasNext()) {
        result->Append(mapper(it->Next(), index, seq));
        index++;
    }

    return result;
}

//Reduce
template <class T, class T2>
T2 Reduce(const Sequence<T>* seq, T2 (*reducer)(const T2&, const T&), T2 init) {
    T2 res = init;
    UniquePtr<Iterator<T>> it(seq->GetIterator());

    while (it->HasNext()) {
        res = reducer(res, it->Next());
    }

    return res;
}

// Where (Фильтрация)
template <class T>
Sequence<T>* Where(const Sequence<T>* seq, bool (*condition)(const T&)) {
    ArraySequence<T>* result = new ArraySequence<T>();
    UniquePtr<Iterator<T>> it(seq->GetIterator());

    while (it->HasNext()) {
        T item = it->Next();
        if (condition(item)) {
            result->Append(item);
        }
    }

    return result;
}