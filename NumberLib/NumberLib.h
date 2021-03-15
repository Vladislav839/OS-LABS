#pragma once

#ifdef NumberLib
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif

class DECLSPEC Number {
	double value;
public:
	Number(double data);
	Number operator+(const Number&);
	Number operator-(const Number&);
	Number operator*(const Number&);
	Number operator/(const Number&);
	~Number();
	double getValue();
};

extern DECLSPEC double zero; 
extern DECLSPEC double one;

DECLSPEC Number CreateNumber(double value);