#pragma once
#include <cstddef>

template <class T>
class ICollection {
public:
    ICollection() = default;
    ICollection(const ICollection& other) = default;
    virtual ~ICollection() = default;

    virtual size_t GetCount() const = 0;
    virtual T Get(size_t index) const = 0;
};