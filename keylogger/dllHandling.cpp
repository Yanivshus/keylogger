#include "dllHandling.h"

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

boolean inject(const std::string& dllName)
{
    return boolean();
}
