#pragma once
#include "Sequence.h"
#include "LinkedList.h"
#include "Exceptions.h"
#include "UniquePtr.h"

template <class T>
class ImmutableListSequence : public Sequence<T> {
private:
    LinkedList<T>* list;

    explicit ImmutableListSequence(LinkedList<T>* lst)
        : list(lst) {
    }

public:
    ImmutableListSequence()
        : list(new LinkedList<T>()) {
    }

    ImmutableListSequence(const T* items, int count)
        : list(new LinkedList<T>(items, count)) {
    }

    ImmutableListSequence(const ImmutableListSequence<T>& other)
        : list(new LinkedList<T>(*other.list)) {
    }

    ImmutableListSequence<T>& operator=(const ImmutableListSequence<T>& other) {
        if (this != &other) {
            delete list;
            list = new LinkedList<T>(*other.list);
        }
        return *this;
    }

    ~ImmutableListSequence() override {
        delete list;
    }

    T GetFirst() const override {
        if (list->GetLength() == 0) {
            throw IndexOutOfRange("Последовательность пуста");
        }
        return list->GetFirst();
    }

    T GetLast() const override {
        if (list->GetLength() == 0) {
            throw IndexOutOfRange("Последовательность пуста");
        }
        return list->GetLast();
    }

    T Get(int index) const override {
        return list->Get(index);
    }

    int GetLength() const override {
        return list->GetLength();
    }

    T operator[](int index) const override {
        return list->Get(index);
    }

    T& operator[](int index) override {
        throw IndexOutOfRange("Невозможно изменить элемент в неизменяемой последовательности");
    }

    Sequence<T>* Append(const T& item) override {
        LinkedList<T>* newList = new LinkedList<T>();

        UniquePtr<Iterator<T>> it(list->GetIterator());
        while (it->HasNext()) {
            newList->Append(it->Next());
        }

        newList->Append(item);

        return new ImmutableListSequence<T>(newList);
    }

    Sequence<T>* Prepend(const T& item) override {
        LinkedList<T>* newList = new LinkedList<T>();

        newList->Append(item);

        UniquePtr<Iterator<T>> it(list->GetIterator());
        while (it->HasNext()) {
            newList->Append(it->Next());
        }

        return new ImmutableListSequence<T>(newList);
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        int size = list->GetLength();
        if (index < 0 || index > size) {
            throw IndexOutOfRange("Индекс вне диапазона");
        }

        LinkedList<T>* newList = new LinkedList<T>();
        UniquePtr<Iterator<T>> it(list->GetIterator());
        int currentIndex = 0;

        while (it->HasNext()) {
            if (currentIndex == index) {
                newList->Append(item);
            }
            newList->Append(it->Next());
            currentIndex++;
        }

        if (index == size) {
            newList->Append(item);
        }

        return new ImmutableListSequence<T>(newList);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        return new ImmutableListSequence<T>(list->GetSubList(startIndex, endIndex));
    }

    Sequence<T>* Concat(Sequence<T>* other) const override {
        LinkedList<T>* newList = new LinkedList<T>();

        UniquePtr<Iterator<T>> it1(list->GetIterator());
        while (it1->HasNext()) {
            newList->Append(it1->Next());
        }

        UniquePtr<Iterator<T>> it2(other->GetIterator());
        while (it2->HasNext()) {
            newList->Append(it2->Next());
        }

        return new ImmutableListSequence<T>(newList);
    }

    Iterator<T>* GetIterator() const override {
        return list->GetIterator();
    }
};