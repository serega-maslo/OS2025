#include <iostream>
#include <windows.h>
#include <string>
#include <tlhelp32.h>

DWORD LaunchProcess(std::string cmdLine, bool wait = false) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(&pi));

    char* cmd = _strdup(cmdLine.c_str());

    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "[User] Ошибка запуска: " << cmdLine << std::endl;
        free(cmd);
        return 0;
    }

    if (wait) {
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    
    DWORD pid = pi.dwProcessId;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    free(cmd);

    return pid;
}

bool IsProcessRunning(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess == NULL) return false;
    
    DWORD exitCode;
    if (GetExitCodeProcess(hProcess, &exitCode)) {
        CloseHandle(hProcess);
        return (exitCode == STILL_ACTIVE);
    }
    CloseHandle(hProcess);
    return false;
}

int main() {
    std::string cmd; 
    HANDLE hNoteProcess3 = NULL;
    
    SetEnvironmentVariable("PROC_TO_KILL", "notepad.exe,calc.exe");
    
    DWORD notePid1 = LaunchProcess("notepad.exe");
    Sleep(100); 

    if (IsProcessRunning(notePid1)) 
        std::cout << "[User] Notepad running (PID: " << notePid1 << ")" << std::endl;

    LaunchProcess("Killer.exe", true);

    if (!IsProcessRunning(notePid1)) 
        std::cout << "[User] SUCCESS: Notepad killed via ENV." << std::endl;
    else 
        std::cerr << "[User] ERROR: Notepad still alive." << std::endl;

    SetEnvironmentVariable("PROC_TO_KILL", NULL);


    DWORD notePid2 = LaunchProcess("notepad.exe");
    Sleep(100);

    LaunchProcess("Killer.exe --name notepad.exe", true);

    if (!IsProcessRunning(notePid2)) 
        std::cout << "[User] SUCCESS: Notepad killed via --name." << std::endl;
    else 
        std::cerr << "[User] ERROR: Notepad still alive." << std::endl;


    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(&pi));

    char* cmdNote = _strdup("notepad.exe");

    if (!CreateProcess(NULL, cmdNote, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "[User] Ошибка запуска Блокнота для Теста 3." << std::endl;
        free(cmdNote);
        goto cleanup;
    }
    free(cmdNote);
    
    hNoteProcess3 = pi.hProcess;
    CloseHandle(pi.hThread);
    DWORD notePid3 = pi.dwProcessId;
    
    Sleep(2000); 
    
    if (!IsProcessRunning(notePid3)) {
         std::cerr << "[User] ОШИБКА СИНХРОНИЗАЦИИ: Блокнот с PID " << notePid3 << " не был запущен." << std::endl;
         goto cleanup;
    }

    std::cout << "[User] Цель PID: " << notePid3 << std::endl;

    cmd = "Killer.exe --id " + std::to_string(notePid3);
    LaunchProcess(cmd, true);

    if (!IsProcessRunning(notePid3)) 
        std::cout << "[User] SUCCESS: Notepad killed via --id." << std::endl;
    else 
        std::cerr << "[User] ОШИБКА: Notepad жив." << std::endl;

cleanup:
    if (hNoteProcess3) CloseHandle(hNoteProcess3);
    
    std::cin.get();
    return 0;
}