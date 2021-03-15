#pragma once
#include <NumberLib.h>

#ifdef VectorLib
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif

class DECLSPEC Vector {
	Number x, y;
public:
	Vector(Number a, Number b);
	Number getX();
	Number getY();
	~Vector();
};

extern DECLSPEC HMODULE hModuleADLL;
DECLSPEC HMODULE GetHandleDLL(void);