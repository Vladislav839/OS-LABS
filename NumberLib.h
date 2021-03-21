#pragma once

class Number {
    double value;

public:
    Number (double data);
    Number();
    Number operator+ (const Number&);
    Number operator- (const Number&);
    Number operator* (const Number&);
    Number operator/ (const Number&);
    ~Number();
    double getValue();
};

extern Number zero;
extern Number one;
Number CreateNumber(double value);