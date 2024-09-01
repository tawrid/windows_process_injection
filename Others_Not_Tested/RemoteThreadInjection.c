#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

DWORD FindProcessId(const char* processName) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    DWORD result = 0;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hProcessSnap) return 0;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hProcessSnap, &pe32)) {
        do {
            if (strcmp(pe32.szExeFile, processName) == 0) {
                result = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <target_process_name> <path_to_dll>\n", argv[0]);
        return 1;
    }

    const char* processName = argv[1];
    const char* dllPath = argv[2];

    DWORD processId = FindProcessId(processName);
    if (processId == 0) {
        printf("Process not found.\n");
        return 1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        printf("Failed to open target process.\n");
        return 1;
    }

    LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pDllPath == NULL) {
        printf("Failed to allocate memory in target process.\n");
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, strlen(dllPath) + 1, NULL)) {
        printf("Failed to write DLL path to target process.\n");
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(
        hProcess, 
        NULL, 
        0, 
        (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"), 
        pDllPath, 
        0, 
        NULL
    );

    if (hThread == NULL) {
        printf("Failed to create remote thread.\n");
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    printf("Remote thread injection executed successfully.\n");
    return 0;
}
