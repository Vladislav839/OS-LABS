// NumberLib.cpp : Определяет экспортируемые функции для DLL.
//

#include "framework.h"
#include "NumberLib.h"

NUMBERLIB_API Number one;
NUMBERLIB_API Number zero;

Number::Number(double data)
{
	value = data;
}

Number::Number()
{
	value = 0.0;
}

Number Number::operator+(const Number& num)
{
	Number res(0.0);
	res.value = value + num.value;
	return res;
}

Number Number::operator-(const Number& num)
{
	Number res(0.0);
	res.value = value + num.value;
	return res;
}

Number Number::operator*(const Number& num)
{
	Number res(0.0);
	res.value = value * num.value;
	return res;
}

Number Number::operator/(const Number& num)
{
	Number res(0.0);
	res.value = value / num.value;
	return res;
}

Number::~Number()
{
}

double Number::getValue()
{
	return this->value;
}

NUMBERLIB_API Number CreateNumber(double value)
{
	return Number(value);
}
