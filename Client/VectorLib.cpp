#include <iostream>
#include <windows.h>
#include "NumberLib.h"
#include "VectorLib.h"

DECLSPEC HMODULE hModuleDLL;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hModuleDLL = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


Vector::Vector(Number a, Number b) : x(a), y(b) {}

Number Vector::getX()
{
	return x;
}

Number Vector::getY()
{
	return y;
}

Vector::~Vector()
{
}

DECLSPEC HMODULE GetHandleDLL(void)
{
	return hModuleDLL;
}
