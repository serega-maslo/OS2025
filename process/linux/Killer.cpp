#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cctype>

void KillProcessByID(pid_t pid) {
    if (kill(pid, SIGTERM) == 0) {
        std::cout << "[Killer] PID " << pid << " terminated." << std::endl;
    } else {
        std::cerr << "[Killer] Error terminating PID " << pid << std::endl;
    }
}

std::string GetProcessName(pid_t pid) {
    std::ifstream commFile("/proc/" + std::to_string(pid) + "/comm");
    if (commFile.is_open()) {
        std::string name;
        std::getline(commFile, name);
        commFile.close();
        return name;
    }
    return "";
}

bool isNumeric(const char* str) {
    if (!str || *str == '\0') return false;
    while (*str) {
        if (!isdigit(*str)) return false;
        str++;
    }
    return true;
}

void KillProcessByName(const std::string& processName) {
    DIR* dir = opendir("/proc");
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (isNumeric(entry->d_name)) {
            pid_t pid = std::stoi(entry->d_name);
            std::string name = GetProcessName(pid);
            if (!name.empty() && name == processName) {
                std::cout << "[Killer] Found '" << processName << "' (PID: " << pid << ")..." << std::endl;
                KillProcessByID(pid);
            }
        }
    }
    closedir(dir);
}

void HandleEnvVarKill() {
    const char* envVar = getenv("PROC_TO_KILL");
    if (envVar) {
        std::string envStr = envVar;
        std::stringstream ss(envStr);
        std::string segment;
        
        while (std::getline(ss, segment, ',')) {
            if (!segment.empty()) {
                KillProcessByName(segment);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc >= 3) {
        std::string argType = argv[1];
        std::string argValue = argv[2];

        if (argType == "--id") {
            pid_t pid = std::stoi(argValue);
            KillProcessByID(pid);
        } 
        else if (argType == "--name") {
            KillProcessByName(argValue);
        }
    }

    HandleEnvVarKill();

    return 0;
}