#pragma once

class Bit {
private:
    bool value;

public:
    explicit Bit(bool v = false) : value(v) {}
    explicit Bit(int v) : value(v != 0) {}

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