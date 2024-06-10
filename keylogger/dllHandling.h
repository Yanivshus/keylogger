#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

#define STR_SIZE 1024

DWORD GetProcessIdByName(const std::wstring& processName);
boolean inject(const std::string& dllName);