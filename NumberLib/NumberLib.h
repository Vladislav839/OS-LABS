#pragma once
#ifdef NUMBERLIB_EXPORTS
#define NUMBERLIB_API __declspec(dllexport)
#else
#define NUMBERLIB_API __declspec(dllimport)
#endif

// Этот класс экспортирован из библиотеки DLL
class NUMBERLIB_API Number {
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

extern NUMBERLIB_API Number zero;
extern NUMBERLIB_API Number one;

NUMBERLIB_API Number CreateNumber(double value);
