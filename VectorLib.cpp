#include "VectorLib.h"

#include <cmath>

Vector::Vector() {
    x = Number(0.0);
    y = Number(0.0);
}

Vector::Vector(Number x, Number y) {
    this->x = x;
    this->y = y;
}

Number Vector::getX() {
    return x;
}

Number Vector::getY() {
    return y;
}

Number Vector::get_Polar_Phi() {
    return Number(atan(y.getValue() / x.getValue()));
}

Number Vector::get_Polar_R() {
    return Number(sqrt((x*x + y*y).getValue()));
}

Vector Vector::add(Vector &vec) {
    return Vector(vec.getX() + x, vec.getY() + y);
}

Vector CreateInstance(Number x, Number y){
    return Vector(x, y);
}