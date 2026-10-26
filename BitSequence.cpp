#include "BitSequence.h"
#include "ArraySequence.h"
#include "Exceptions.h"

Sequence<Bit>* BitSequence::Instance() {
    return this; 
}

Sequence<Bit>* BitSequence::Clone() const {
    return new BitSequence(*this); 
}

Sequence<Bit>* BitSequence::EmptyClone() const {
    return new BitSequence();
}

void BitSequence::AppendInternal(const Bit& item) {
    seq->Append(item);
}

void BitSequence::PrependInternal(const Bit& item) {
    seq->Prepend(item);
}

void BitSequence::InsertAtInternal(const Bit& item, size_t index) {
    seq->InsertAt(item, index);
}

BitSequence::BitSequence() {
    seq = new MutableArraySequence<Bit>();
}

BitSequence::BitSequence(const BitSequence& other) {
    seq = new MutableArraySequence<Bit>();
    for (size_t i = 0; i < other.GetCount(); ++i) {
        seq->Append(other.Get(i));
    }
}

BitSequence& BitSequence::operator=(const BitSequence& other) {
    if (this != &other) {
        delete seq;
        seq = new MutableArraySequence<Bit>();
        for (size_t i = 0; i < other.GetCount(); ++i) {
            seq->Append(other.Get(i));
        }
    }
    return *this;
}

BitSequence::~BitSequence() {
    delete seq;
}

size_t BitSequence::GetCount() const {
    return seq->GetCount();
}

Bit BitSequence::Get(size_t index) const {
    return seq->Get(index);
}

Bit BitSequence::GetFirst() const {
    if (seq->GetCount() == 0) {
        throw IndexOutOfRange("Битовая последовательность пуста");
    }
    return seq->GetFirst();
}

Bit BitSequence::GetLast() const {
    if (seq->GetCount() == 0) {
        throw IndexOutOfRange("Битовая последовательность пуста");
    }
    return seq->GetLast();
}

Bit BitSequence::operator[](size_t index) const {
    return (*seq)[index];
}

Bit& BitSequence::operator[](size_t index) {
    return (*seq)[index];
}

Sequence<Bit>* BitSequence::Append(const Bit& item) {
    BitSequence* inst = static_cast<BitSequence*>(this->Instance());
    inst->AppendInternal(item);
    return inst;
}

Sequence<Bit>* BitSequence::Prepend(const Bit& item) {
    BitSequence* inst = static_cast<BitSequence*>(this->Instance());
    inst->PrependInternal(item);
    return inst;
}

Sequence<Bit>* BitSequence::InsertAt(const Bit& item, size_t index) {
    BitSequence* inst = static_cast<BitSequence*>(this->Instance());
    inst->InsertAtInternal(item, index);
    return inst;
}

Sequence<Bit>* BitSequence::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex >= GetCount() || endIndex >= GetCount() || startIndex > endIndex) {
        throw IndexOutOfRange("GetSubsequence: Invalid bounds");
    }
    
    BitSequence* result = static_cast<BitSequence*>(this->EmptyClone());
    for (size_t i = startIndex; i <= endIndex; ++i) {
        result->AppendInternal(this->Get(i));
    }
    return result;
}

Sequence<Bit>* BitSequence::Concat(const Sequence<Bit>* other) const {
    if (other == nullptr) {
        throw InvalidArgument("Concat: other sequence is null");
    }

    BitSequence* result = static_cast<BitSequence*>(this->Clone());

    IEnumerator<Bit>* it = other->GetEnumerator();
    while (it->MoveNext()) {
        result->AppendInternal(it->GetCurrent());
    }
    delete it;

    return result;
}

IEnumerator<Bit>* BitSequence::GetEnumerator() const {
    return seq->GetEnumerator();
}

BitSequence* BitSequence::And(const BitSequence& other) const {
    BitSequence* result = static_cast<BitSequence*>(this->EmptyClone());
    size_t len = this->GetCount() < other.GetCount() ? this->GetCount() : other.GetCount();

    for (size_t i = 0; i < len; ++i) {
        result->AppendInternal(this->Get(i) & other.Get(i));
    }
    return result;
}

BitSequence* BitSequence::Or(const BitSequence& other) const {
    BitSequence* result = static_cast<BitSequence*>(this->EmptyClone());
    size_t len = this->GetCount() < other.GetCount() ? this->GetCount() : other.GetCount();

    for (size_t i = 0; i < len; ++i) {
        result->AppendInternal(this->Get(i) | other.Get(i));
    }
    return result;
}

BitSequence* BitSequence::Xor(const BitSequence& other) const {
    BitSequence* result = static_cast<BitSequence*>(this->EmptyClone());
    size_t len = this->GetCount() < other.GetCount() ? this->GetCount() : other.GetCount();

    for (size_t i = 0; i < len; ++i) {
        result->AppendInternal(this->Get(i) ^ other.Get(i));
    }
    return result;
}

BitSequence* BitSequence::Not() const {
    BitSequence* result = static_cast<BitSequence*>(this->EmptyClone());

    for (size_t i = 0; i < this->GetCount(); ++i) {
        result->AppendInternal(~this->Get(i));
    }
    return result;
}