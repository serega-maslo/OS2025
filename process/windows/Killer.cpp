#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <sstream>

HANDLE OpenProcessForKill(DWORD pid) {
    HANDLE h = OpenProcess(
        PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE,
        pid
    );
    if (!h) {
        h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }
    return h;
}

void KillProcessByID(DWORD pid) {
    HANDLE h = OpenProcessForKill(pid);
    if (!h) {
        std::cerr << "[Killer] Can't open PID " << pid << std::endl;
        return;
    }
    TerminateProcess(h, 0);
    CloseHandle(h);
    std::cout << "[Killer] Killed PID " << pid << std::endl;
}

void KillProcessByName(const std::string& name) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    if (Process32First(snap, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, name.c_str()) == 0)
                KillProcessByID(pe.th32ProcessID);
        } while (Process32Next(snap, &pe));
    }
    CloseHandle(snap);
}

int main(int argc, char* argv[]) {
    if (argc >= 3) {
        std::string t = argv[1], v = argv[2];
        if (t == "--id") KillProcessByID(std::stoul(v));
        else if (t == "--name") KillProcessByName(v);
    }

    char buf[32767];
    if (GetEnvironmentVariable("PROC_TO_KILL", buf, 32767) > 0) {
        std::stringstream ss(buf);
        std::string name;
        while (std::getline(ss, name, ',')) {
            if (!name.empty()) KillProcessByName(name);
        }
    }
    return 0;
}
