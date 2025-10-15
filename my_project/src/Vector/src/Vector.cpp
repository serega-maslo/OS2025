#define VECTOR_EXPORTS 

#include "Vector.h"

Vector Vector::add(const Vector& other) const {
    Number new_x = x_coord + other.x_coord;
    Number new_y = y_coord + other.y_coord;
    return Vector(new_x, new_y);
}

Number Vector::getPolarR() const {
    Number x_sq = x_coord * x_coord;
    Number y_sq = y_coord * y_coord;
    Number r_sq = x_sq + y_sq;
    return createNumber(std::sqrt(r_sq.value));
}

Number Vector::getPolarTheta() const {
    return createNumber(std::atan2(y_coord.value, x_coord.value));
}

const Vector VECTOR_ZERO(ZERO, ZERO);
const Vector VECTOR_ONE(ONE, ONE);
