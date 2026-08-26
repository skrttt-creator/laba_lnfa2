#pragma once

template <class T>
class Iterator {
public:
    virtual ~Iterator() = default;

    virtual bool HasNext() const = 0;

    virtual const T& Next() = 0;

    virtual void Reset() = 0;
};

