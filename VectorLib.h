#pragma once

#include "NumberLib.h"

class Vector {
public:
    Vector();
    Vector(Number x, Number y);
    Number getX();
    Number getY();
    Number get_Polar_R();
    Number get_Polar_Phi();
    Vector add(Vector& vec);
private:
    Number x;
    Number y;
};

Vector CreateInstance(Number x, Number y);