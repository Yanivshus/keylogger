#include "dllHandling.h"

boolean inject(const std::string& dllName, std::string& procName)
{
    DWORD err;
    char dllFull[STR_SIZE] = {};
    // Get full path of DLL to inject
    DWORD pathLen = GetFullPathNameA("mydll.dll", STR_SIZE, dllFull, NULL);

    // Get LoadLibrary function address –
    // the address doesn't change at remote process
    PVOID addrLoadLibrary = (PVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");//LoadLibraryA


    // Open remote process
    DWORD id = GetProcessIdByName(L"Notepad.exe"); // getting the pid of the process

    HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
    if (proc == NULL) {
        std::cout << "couldnt open process" << std::endl;
        return false;
    }

    // Get a pointer to memory location in remote process,
    // big enough to store DLL path
    PVOID memAddr = (PVOID)VirtualAllocEx(proc, 0, strlen(dllFull), MEM_COMMIT, PAGE_READWRITE);
    if (NULL == memAddr) {
        std::cout << "couldnt alloc" << std::endl;
        err = GetLastError();
        return false;
    }

    // Write DLL name to remote process memory
    BOOL check = WriteProcessMemory(proc, memAddr, dllFull, strlen(dllFull), NULL);
    if (0 == check) {
        err = GetLastError();
        std::cout << "not working" << std::endl;
        return false;

    }

    // Open remote thread, while executing LoadLibrary
    // with parameter DLL name, will trigger DLLMain
    HANDLE hRemote = CreateRemoteThread(proc, 0, 0, (LPTHREAD_START_ROUTINE)addrLoadLibrary, memAddr, 0, 0);
    if (NULL == hRemote) {
        std::cout << "couldnt create remote thread" << std::endl;
        err = GetLastError();
        return false;
    }
    WaitForSingleObject(hRemote, INFINITE);
    check = CloseHandle(hRemote);
    return true;
}


DWORD GetProcessIdByName(const std::wstring& processName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // Check if the snapshot handle is valid
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        // Initialize a structure to store information about a process in the snapshot
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(processEntry);

        if (Process32First(hSnapshot, &processEntry))
        {
            // Iterate through the list of processes in the snapshot
            do {
                // Compare the name of the current process with the specified process name
                if (_wcsicmp(processEntry.szExeFile, processName.c_str()) == 0) {

                    return processEntry.th32ProcessID;
                }
                // Move to the next process in the snapshot
            } while (Process32Next(hSnapshot, &processEntry));
        }
        CloseHandle(hSnapshot);
    }
}

