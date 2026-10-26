#pragma once
#include "Exceptions.h"
#include "IEnumerator.h"
#include "IEnumerable.h"
#include <cstddef>

template <class T>
class LinkedList : public IEnumerable<T> {
public:
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& v) : 
            value(v), next(nullptr) {
        }
    };

private:
    Node* head;
    Node* tail;
    size_t length;

    class LinkedListEnumerator : public IEnumerator<T> {
    private:
        const LinkedList<T>* list;
        Node* current;
        bool started;
    public:
        explicit LinkedListEnumerator(const LinkedList<T>* lst);
        bool MoveNext() override;
        T GetCurrent() const override;
        void Reset() override;
    };

public:
    LinkedList();
    LinkedList(const T* items, size_t count);
    LinkedList(const LinkedList<T>& other);
    LinkedList<T>& operator=(const LinkedList<T>& other);
    ~LinkedList();

    void Append(const T& val);
    void Prepend(const T& val);
    void InsertAt(const T& val, size_t index);

    T GetFirst() const;
    T GetLast() const;
    T Get(size_t index) const;
    T& GetRef(size_t index);

    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) const;
    LinkedList<T>* Concat(const LinkedList<T>* other) const;

    size_t GetLength() const;

    IEnumerator<T>* GetEnumerator() const override;
};

#include "LinkedList.tpp"