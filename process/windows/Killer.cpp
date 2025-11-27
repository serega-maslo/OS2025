#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <sstream>

void KillProcessByID(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        DWORD error = GetLastError(); 
        
        if (error == 87) {
             std::cerr << "[Killer] PID " << pid << " not found (already closed or PID recycled)." << std::endl;
        } else {
             std::cerr << "[Killer] Failed to open PID: " << pid << ". Error Code: " << error << std::endl;
        }
        return;
    }
    
    if (TerminateProcess(hProcess, 0)) {
        std::cout << "[Killer] PID " << pid << " terminated." << std::endl;
    } else {
        std::cerr << "[Killer] Error terminating PID " << pid << std::endl;
    }
    CloseHandle(hProcess);
}

void KillProcessByName(const std::string& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, processName.c_str()) == 0) {
                std::cout << "[Killer] Found '" << processName << "' (PID: " << pe32.th32ProcessID << ")..." << std::endl;
                KillProcessByID(pe32.th32ProcessID);
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
}

int main(int argc, char* argv[]) {
    if (argc >= 3) {
        std::string argType = argv[1];
        std::string argValue = argv[2];

        if (argType == "--id") {
            DWORD pid = std::stoul(argValue);
            KillProcessByID(pid);
        } 
        else if (argType == "--name") {
            KillProcessByName(argValue);
        }
    }

    const int bufferSize = 32767;
    char buffer[bufferSize];
    if (GetEnvironmentVariable("PROC_TO_KILL", buffer, bufferSize) > 0) {
        std::string envStr = buffer;
        std::stringstream ss(envStr);
        std::string segment;
        
        while (std::getline(ss, segment, ',')) {
            if (!segment.empty()) {
                KillProcessByName(segment);
            }
        }
    }

    return 0;
}