#pragma once
#include "Sequence.h"
#include "Bit.h"
#include <cstddef>

class BitSequence : public Sequence<Bit> {
private:
    Sequence<Bit>* seq;

protected:
    Sequence<Bit>* Instance() override;
    Sequence<Bit>* Clone() const override;
    Sequence<Bit>* EmptyClone() const override;

    void AppendInternal(const Bit& item) override;
    void PrependInternal(const Bit& item) override;
    void InsertAtInternal(const Bit& item, size_t index) override;

public:
    BitSequence();
    BitSequence(const BitSequence& other);
    BitSequence& operator=(const BitSequence& other);
    ~BitSequence() override;

    size_t GetCount() const override;
    Bit Get(size_t index) const override;
    Bit GetFirst() const override;
    Bit GetLast() const override;

    Bit operator[](size_t index) const override;
    Bit& operator[](size_t index) override;

    Sequence<Bit>* Append(const Bit& item) override;
    Sequence<Bit>* Prepend(const Bit& item) override;
    Sequence<Bit>* InsertAt(const Bit& item, size_t index) override;

    Sequence<Bit>* GetSubsequence(size_t startIndex, size_t endIndex) const override;
    Sequence<Bit>* Concat(const Sequence<Bit>* other) const override;

    IEnumerator<Bit>* GetEnumerator() const override;

    BitSequence* And(const BitSequence& other) const;
    BitSequence* Or(const BitSequence& other) const;
    BitSequence* Xor(const BitSequence& other) const;
    BitSequence* Not() const;
};