#pragma once
#include "Exceptions.h"
#include "Iterator.h"
#include "UniquePtr.h"

template <class T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& v)
            : value(v), next(nullptr) {
        }
    };

    Node* head;
    Node* tail;
    int length;

public:
    class ListIterator : public Iterator<T> {
    private:
        Node* current;
        Node* start;

    public:
        explicit ListIterator(Node* h)
            : current(h), start(h) {
        }

        bool HasNext() const override {
            return current != nullptr;
        }

        const T& Next() override {
            const T& val = current->value;
            current = current->next;
            return val;
        }

        void Reset() override {
            current = start;
        }
    };

    LinkedList()
        : head(nullptr), tail(nullptr), length(0) {
    }

    LinkedList(const T* items, int count)
        : head(nullptr), tail(nullptr), length(0) {
        for (int i = 0; i < count; ++i) {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T>& other)
        : head(nullptr), tail(nullptr), length(0) {
        Node* curr = other.head;
        while (curr) {
            Append(curr->value);
            curr = curr->next;
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            head = nullptr;
            tail = nullptr;
            length = 0;

            Node* curr = other.head;
            while (curr) {
                Append(curr->value);
                curr = curr->next;
            }
        }
        return *this;
    }

    ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void Append(const T& val) {
        Node* node = new Node(val);
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        length++;
    }

    void Prepend(const T& val) {
        Node* node = new Node(val);
        if (!head) {
            head = node;
            tail = node;
        } else {
            node->next = head;
            head = node;
        }
        length++;
    }

    void InsertAt(const T& val, int index) {
        if (index < 0 || index > length) {
            throw IndexOutOfRange("Ошибка индекса списка");
        }
        if (index == 0) {
            Prepend(val);
            return;
        }
        if (index == length) {
            Append(val);
            return;
        }

        Node* curr = head;
        for (int i = 0; i < index - 1; i++) {
            curr = curr->next;
        }

        Node* node = new Node(val);
        node->next = curr->next;
        curr->next = node;
        length++;
    }

    T GetFirst() const {
        if (!head) {
            throw IndexOutOfRange("Список пуст");
        }
        return head->value;
    }

    T GetLast() const {
        if (!tail) {
            throw IndexOutOfRange("Список пуст");
        }
        return tail->value;
    }

    T Get(int index) const {
        if (index < 0 || index >= length) {
            throw IndexOutOfRange("Ошибка индекса списка");
        }
        Node* curr = head;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        }
        return curr->value;
    }

    T& GetRef(int index) {
        if (index < 0 || index >= length) {
            throw IndexOutOfRange("Ошибка индекса списка");
        }
        Node* curr = head;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        }
        return curr->value;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex) {
            throw IndexOutOfRange("Ошибка индекса подсписка");
        }

        LinkedList<T>* subList = new LinkedList<T>();
        Node* curr = head;

        for (int i = 0; i < startIndex; i++) {
            curr = curr->next;
        }
        for (int i = startIndex; i <= endIndex; i++) {
            subList->Append(curr->value);
            curr = curr->next;
        }

        return subList;
    }

    LinkedList<T>* Concat(const LinkedList<T>* other) const {
        LinkedList<T>* result = new LinkedList<T>(*this);
        Node* curr = other->head;
        while (curr) {
            result->Append(curr->value);
            curr = curr->next;
        }
        return result;
    }

    int GetLength() const {
        return length;
    }

    Node* GetHead() const {
        return head;
    }

    Iterator<T>* GetIterator() const {
        return new ListIterator(head);
    }
};