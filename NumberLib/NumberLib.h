#pragma once

#define WIN32_LEAN_AND_MEAN            
#include <windows.h>

class Number {
	double value;
public:
	Number(double data);
	Number();
	Number operator+(const Number&);
	Number operator-(const Number&);
	Number operator*(const Number&);
	Number operator/(const Number&);
	~Number();
	double getValue();
};

extern Number zero = Number(0.0);
extern Number one = Number(1.0);
Number CreateNumber(double value);
