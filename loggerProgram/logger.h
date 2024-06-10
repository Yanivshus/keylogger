#pragma once
#ifdef DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#include <Windows.h>
extern "C"
{
	DECLDIR void connectAndLog();
}
