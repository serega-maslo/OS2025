#ifndef VECTOR_H
#define VECTOR_H

#include "Number.h"
#include <cmath>

#ifdef _WIN32
  #ifdef VECTOR_EXPORTS
    #define VECTOR_API __declspec(dllexport)
  #else
    #define VECTOR_API __declspec(dllimport)
  #endif
#else
  #define VECTOR_API
#endif

class VECTOR_API Vector {
private:
    Number x_coord;
    Number y_coord;

public:
    Vector(const Number& x, const Number& y) : x_coord(x), y_coord(y) {}

    Vector add(const Vector& other) const;

    Number getPolarR() const;
    Number getPolarTheta() const;

    Number getX() const { return x_coord; }
    Number getY() const { return y_coord; }
};

extern VECTOR_API const Vector VECTOR_ZERO;
extern VECTOR_API const Vector VECTOR_ONE;

#endif // VECTOR_H
