#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <wchar.h> 

DWORD FindProcessId(const std::wstring& processName) {
    PROCESSENTRY32W processInfo{};
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to create toolhelp snapshot. Error code: " << GetLastError() << std::endl;
        return 0;
    }

    if (Process32FirstW(processesSnapshot, &processInfo)) {
        do {
            if (_wcsicmp(processInfo.szExeFile, processName.c_str()) == 0) {
                CloseHandle(processesSnapshot);
                return processInfo.th32ProcessID;
            }
        } while (Process32NextW(processesSnapshot, &processInfo));
    }
    else {
        std::cerr << "Error: Unable to get first process. Error code: " << GetLastError() << std::endl;
    }

    CloseHandle(processesSnapshot);
    return 0;
}

bool InjectDLL(DWORD processId, const std::wstring& dllPath) {
    if (processId == 0) {
        std::cerr << "Error: Invalid Process ID (0)." << std::endl;
        return false;
    }
    if (dllPath.empty()) {
        std::cerr << "Error: DLL path is empty." << std::endl;
        return false;
    }

    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION |
        PROCESS_CREATE_THREAD |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE |
        PROCESS_VM_READ,
        FALSE,
        processId
    );

    if (hProcess == NULL) {
        std::wcerr << L"Error: Could not open target process " << processId << L". Error code: " << GetLastError() << std::endl;
        std::wcerr << L"-> Hint: Try running this injector as Administrator." << std::endl;
        return false;
    }
    std::wcout << L"Successfully opened target process " << processId << std::endl;

    LPVOID pLoadLibraryW = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    if (pLoadLibraryW == NULL) {
        CloseHandle(hProcess);
        return false;
    }
    std::wcout << L"Found LoadLibraryW at address: " << pLoadLibraryW << std::endl;

    size_t dllPathSize = (dllPath.length() + 1) * sizeof(wchar_t);
    LPVOID pRemoteDllPath = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pRemoteDllPath == NULL) {
        CloseHandle(hProcess);
        return false;
    }
    std::wcout << L"Allocated " << dllPathSize << L" bytes for DLL path in target process at: " << pRemoteDllPath << std::endl;


    SIZE_T bytesWritten = 0;
    BOOL writeResult = WriteProcessMemory(hProcess, pRemoteDllPath, dllPath.c_str(), dllPathSize, &bytesWritten);
    if (!writeResult || bytesWritten != dllPathSize) {
        VirtualFreeEx(hProcess, pRemoteDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    std::wcout << L"Successfully wrote DLL path to target process memory." << std::endl;

    HANDLE hRemoteThread = CreateRemoteThread(
        hProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)pLoadLibraryW,
        pRemoteDllPath,
        0,
        NULL
    );

    if (hRemoteThread == NULL) {
        VirtualFreeEx(hProcess, pRemoteDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    std::wcout << L"Successfully created remote thread. Waiting for it to finish..." << std::endl;

    WaitForSingleObject(hRemoteThread, INFINITE);
    std::wcout << L"Remote thread finished." << std::endl;

    DWORD remoteThreadExitCode = 0;
    GetExitCodeThread(hRemoteThread, &remoteThreadExitCode);
    if (remoteThreadExitCode == 0) {
        std::wcerr << L"Warning: LoadLibraryW might have failed in the remote process (returned NULL)." << std::endl;
    }
    else {
        std::wcout << L"LoadLibraryW appears to have succeeded in the remote process (returned handle 0x" << std::hex << remoteThreadExitCode << std::dec << L")." << std::endl;
    }

    VirtualFreeEx(hProcess, pRemoteDllPath, 0, MEM_RELEASE);
    CloseHandle(hRemoteThread);
    CloseHandle(hProcess);

    return true;
}


int main() {
    const std::wstring targetProcessName = L"GeforceNow.exe";
    const std::wstring dllName = L"GeforceNowAMD.dll";
    wchar_t injectorPath[MAX_PATH];
    DWORD pathLen = GetModuleFileNameW(NULL, injectorPath, MAX_PATH);

    if (pathLen == 0 || pathLen == MAX_PATH) {
        std::cerr << "Error: Could not get injector module path. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    std::wstring injectorPathStr = injectorPath;
    size_t lastBackslash = injectorPathStr.find_last_of(L"\\/");
    if (lastBackslash == std::wstring::npos) {
        std::cerr << "Error: Could not extract directory from injector path." << std::endl;
        return 1;
    }
    std::wstring injectorDir = injectorPathStr.substr(0, lastBackslash + 1);
    std::wstring fullDllPath = injectorDir + dllName;

    std::wcout << L"Injector path: " << injectorPathStr << std::endl;
    std::wcout << L"Target DLL path: " << fullDllPath << std::endl;
    std::wcout << L"Waiting for process: " << targetProcessName << std::endl;

    DWORD processId = 0;
    while (true) {
        processId = FindProcessId(targetProcessName);
        if (processId != 0) {
            std::wcout << L"\nFound " << targetProcessName << L" running with PID: " << processId << std::endl;
            std::wcout << L"Attempting to inject " << dllName << L"..." << std::endl;

            // Add a small delay before injection attempt, sometimes helps if process just started
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (InjectDLL(processId, fullDllPath)) {
                std::wcout << L"DLL injection initiated successfully." << std::endl;
            }
            else {
                std::wcerr << L"DLL injection failed." << std::endl;
            }
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }

    return 0;
}
