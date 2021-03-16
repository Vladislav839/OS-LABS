#pragma once
#ifdef VECTORLIB_EXPORTS
#define VECTORLIB_API __declspec(dllexport)
#else
#define VECTORLIB_API __declspec(dllimport)
#endif

#include "NumberLib.h"

// Этот класс экспортирован из библиотеки DLL
class VECTORLIB_API VectorApi {
public:
	virtual Number getX() = 0;
	virtual Number getY() = 0;
	virtual Number get_Polar_R() = 0;
	virtual Number get_Polar_Phi() = 0;
	virtual void* add(VectorApi* v) = 0;
	virtual void destroy() = 0;
};


extern "C" VECTORLIB_API VectorApi* __cdecl CreateInstance(Number x, Number y);
