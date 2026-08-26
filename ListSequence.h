#pragma once
#include "Sequence.h"
#include "LinkedList.h"
#include "UniquePtr.h"

template <class T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T>* list;

    explicit ListSequence(LinkedList<T>* existingList)
        : list(existingList) {
    }

public:
    ListSequence()
        : list(new LinkedList<T>()) {
    }

    ListSequence(const T* items, int count)
        : list(new LinkedList<T>(items, count)) {
    }

    ListSequence(const ListSequence<T>& other)
        : list(new LinkedList<T>(*other.list)) {
    }

    ListSequence<T>& operator=(const ListSequence<T>& other) {
        if (this != &other) {
            delete list;
            list = new LinkedList<T>(*other.list);
        }
        return *this;
    }

    ~ListSequence() override {
        delete list;
    }

    T GetFirst() const override {
        return list->GetFirst();
    }

    T GetLast() const override {
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
        return list->GetRef(index);
    }

    Sequence<T>* Append(const T& item) override {
        list->Append(item);
        return this;
    }

    Sequence<T>* Prepend(const T& item) override {
        list->Prepend(item);
        return this;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        list->InsertAt(item, index);
        return this;
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        return new ListSequence<T>(list->GetSubList(startIndex, endIndex));
    }

    Sequence<T>* Concat(Sequence<T>* other) const override {
        ListSequence<T>* result = new ListSequence<T>();

        UniquePtr<Iterator<T>> it1(this->GetIterator());
        while (it1->HasNext()) {
            result->Append(it1->Next());
        }

        UniquePtr<Iterator<T>> it2(other->GetIterator());
        while (it2->HasNext()) {
            result->Append(it2->Next());
        }

        return result;
    }

    Iterator<T>* GetIterator() const override {
        return new typename LinkedList<T>::ListIterator(list->GetHead());
    }
};