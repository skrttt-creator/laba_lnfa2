#pragma once
#include "ArraySequence.h"
#include "Exceptions.h"
#include "UniquePtr.h"

class Bit {
private:
    bool value;

public:
    explicit Bit(bool v = false)
        : value(v) {
    }

    explicit Bit(int v)
        : value(v != 0) {
    }

    bool Get() const {
        return value;
    }

    Bit operator&(const Bit& other) const {
        return Bit(value & other.value);
    }

    Bit operator|(const Bit& other) const {
        return Bit(value | other.value);
    }

    Bit operator^(const Bit& other) const {
        return Bit(value ^ other.value);
    }

    Bit operator~() const {
        return Bit(!value);
    }
};

class BitSequence : public Sequence<Bit> {
private:
    ArraySequence<Bit>* seq;

public:
    BitSequence()
        : seq(new ArraySequence<Bit>()) {
    }

    BitSequence(const BitSequence& other)
        : seq(new ArraySequence<Bit>(*other.seq)) {
    }

    BitSequence& operator=(const BitSequence& other) {
        if (this != &other) {
            delete seq;
            seq = new ArraySequence<Bit>(*other.seq);
        }
        return *this;
    }

    ~BitSequence() override {
        delete seq;
    }

    Bit GetFirst() const override {
        if (seq->GetLength() == 0) {
            throw IndexOutOfRange("Битовая последовательность пуста");
        }
        return seq->GetFirst();
    }

    Bit GetLast() const override {
        if (seq->GetLength() == 0) {
            throw IndexOutOfRange("Битовая последовательность пуста");
        }
        return seq->GetLast();
    }

    Bit Get(int index) const override {
        return seq->Get(index);
    }

    int GetLength() const override {
        return seq->GetLength();
    }

    Bit operator[](int index) const override {
        return (*seq)[index];
    }

    Bit& operator[](int index) override {
        return (*seq)[index];
    }

    Sequence<Bit>* Append(const Bit& item) override {
        seq->Append(item);
        return this;
    }

    Sequence<Bit>* Prepend(const Bit& item) override {
        seq->Prepend(item);
        return this;
    }

    Sequence<Bit>* InsertAt(const Bit& item, int index) override {
        seq->InsertAt(item, index);
        return this;
    }

    Sequence<Bit>* GetSubsequence(int startIndex, int endIndex) const override {
        BitSequence* result = new BitSequence();

        for (int i = startIndex; i <= endIndex; ++i) {
            result->Append(this->Get(i));
        }

        return result;
    }

    Sequence<Bit>* Concat(Sequence<Bit>* list) const override {
        BitSequence* result = new BitSequence();

        for (int i = 0; i < this->GetLength(); ++i) {
            result->Append(this->Get(i));
        }

        UniquePtr<Iterator<Bit>> it(list->GetIterator());
        while (it->HasNext()) {
            result->Append(it->Next());
        }

        return result;
    }

    Iterator<Bit>* GetIterator() const override {
        return seq->GetIterator();
    }

    BitSequence* And(const BitSequence& other) const {
        BitSequence* result = new BitSequence();
        int len = this->GetLength() < other.GetLength() ? this->GetLength() : other.GetLength();

        for (int i = 0; i < len; ++i) {
            result->Append(this->Get(i) & other.Get(i));
        }

        return result;
    }

    BitSequence* Or(const BitSequence& other) const {
        BitSequence* result = new BitSequence();
        int len = this->GetLength() < other.GetLength() ? this->GetLength() : other.GetLength();

        for (int i = 0; i < len; ++i) {
            result->Append(this->Get(i) | other.Get(i));
        }

        return result;
    }

    BitSequence* Xor(const BitSequence& other) const {
        BitSequence* result = new BitSequence();
        int len = this->GetLength() < other.GetLength() ? this->GetLength() : other.GetLength();

        for (int i = 0; i < len; ++i) {
            result->Append(this->Get(i) ^ other.Get(i));
        }

        return result;
    }

    BitSequence* Not() const {
        BitSequence* result = new BitSequence();

        for (int i = 0; i < this->GetLength(); ++i) {
            result->Append(~this->Get(i));
        }

        return result;
    }
};