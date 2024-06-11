#pragma once
#ifdef DLL_EXPORT
#define _CRT_SECURE_NO_WARNINGS
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#include <Windows.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <Psapi.h>

extern "C"
{
	DECLDIR void connectAndLog();
}
