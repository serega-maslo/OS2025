#include "Number.h"


Number Number::operator+(const Number& other) const {
    return Number(value + other.value);
}

Number Number::operator-(const Number& other) const {
    return Number(value - other.value);
}

Number Number::operator*(const Number& other) const {
    return Number(value * other.value);
}

Number Number::operator/(const Number& other) const {
    if (other.value == 0.0) {
        return Number(0.0);
    }
    return Number(value / other.value);
}

const Number ZERO(0.0);
const Number ONE(1.0);

Number createNumber(double value) {
    return Number(value);
}
