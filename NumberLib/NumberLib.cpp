#include "NumberLib.h"
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		zero = 0.0;
		one = 1.0;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


Number::Number(double data)
{
	value = data;
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

DECLSPEC double zero; 
DECLSPEC double one;


DECLSPEC Number CreateNumber(double value)
{
	return Number(value);
}
