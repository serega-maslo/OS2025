#ifndef NUMBER_H
#define NUMBER_H


class Number {
public:
    double value;

    Number() : value(0.0) {}
    explicit Number(double val) : value(val) {}

    Number operator+(const Number& other) const;
    Number operator-(const Number& other) const;
    Number operator*(const Number& other) const;
    Number operator/(const Number& other) const;

    double getValue() const { return value; }
};

extern const Number ZERO;
extern const Number ONE;

Number createNumber(double value);

#endif // NUMBER_H
