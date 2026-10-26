#pragma once
#include "LinkedList.h"

template <class T>
LinkedList<T>::LinkedList() 
    : head(nullptr), tail(nullptr), length(0) {
}

template <class T>
LinkedList<T>::LinkedList(const T* items, size_t count) 
    : head(nullptr), tail(nullptr), length(0) {
    for (size_t i = 0; i < count; ++i) {
        Append(items[i]);
    }
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) 
    : head(nullptr), tail(nullptr), length(0) {
    Node* curr = other.head;
    while (curr) {
        Append(curr->value);
        curr = curr->next;
    }   
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
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

template <class T>
LinkedList<T>::~LinkedList() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

template <class T>
void LinkedList<T>::Append(const T& val) {
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

template <class T>
void LinkedList<T>::Prepend(const T& val) {
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

template <class T>
void LinkedList<T>::InsertAt(const T& val, size_t index) {
    if (index > length) {
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
    for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    Node* node = new Node(val);
    node->next = curr->next;
    curr->next = node;
    length++;
}

template <class T>
T LinkedList<T>::GetFirst() const {
    if (!head) {
        throw IndexOutOfRange("Список пуст");
    }
    return head->value;
}

template <class T>
T LinkedList<T>::GetLast() const {
    if (!tail) {
        throw IndexOutOfRange("Список пуст");
    }
    return tail->value;
}

template <class T>
T LinkedList<T>::Get(size_t index) const {
    if (index >= length) {
        throw IndexOutOfRange("Ошибка индекса списка");
    }
    Node* curr = head;
    for (size_t i = 0; i < index; i++) {
        curr = curr->next;
    }
    return curr->value;
}

template <class T>
T& LinkedList<T>::GetRef(size_t index) {
    if (index >= length) {
        throw IndexOutOfRange("IndexOutOfRange");
    }
    
    Node* curr = head; 
    for (size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }
    
    return curr->value;
}

template <class T>
LinkedList<T>* LinkedList<T>::GetSubList(size_t startIndex, size_t endIndex) const {
    if (startIndex >= length || endIndex >= length || startIndex > endIndex) {
        throw IndexOutOfRange("Ошибка индекса подсписка");
    }

    LinkedList<T>* subList = new LinkedList<T>();
    Node* curr = head;

    for (size_t i = 0; i < startIndex; i++) {
        curr = curr->next;
    }
    for (size_t i = startIndex; i <= endIndex; i++) {
        subList->Append(curr->value);
        curr = curr->next;
    }

    return subList;
}

template <class T>
LinkedList<T>* LinkedList<T>::Concat(const LinkedList<T>* other) const {
    LinkedList<T>* result = new LinkedList<T>(*this);
    Node* curr = other->head;
    while (curr) {
        result->Append(curr->value);
        curr = curr->next;
    }
    return result;
}

template <class T>
size_t LinkedList<T>::GetLength() const {
    return length;
}

template <class T>
IEnumerator<T>* LinkedList<T>::GetEnumerator() const {
    return new LinkedListEnumerator(this);
}

template <class T>
LinkedList<T>::LinkedListEnumerator::LinkedListEnumerator(const LinkedList<T>* lst)
    : list(lst), current(nullptr), started(false) {
}

template <class T>
bool LinkedList<T>::LinkedListEnumerator::MoveNext() {
    if (!started) {
        current = list->head;
        started = true;
    } else if (current != nullptr) {
        current = current->next;
    }
    return current != nullptr;
}

template <class T>
T LinkedList<T>::LinkedListEnumerator::GetCurrent() const {
    if (!started || current == nullptr) {
        throw IndexOutOfRange("Итератор вне границ");
    }
    return current->value;
}

template <class T>
void LinkedList<T>::LinkedListEnumerator::Reset() {
    current = nullptr;
    started = false;
}