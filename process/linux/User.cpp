#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdlib>

pid_t LaunchDummyProcess(const char* name, const char* arg) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "[User] fork failed." << std::endl;
        return 0;
    } 
    else if (pid == 0) {
        execlp(name, name, arg, (char*)NULL);
        std::cerr << "[User] execlp failed for " << name << std::endl;
        exit(EXIT_FAILURE);
    }
    return pid;
}

bool IsProcessRunning(pid_t pid) {
    return kill(pid, 0) == 0;
}

int main() {
    
    std::cout << "\n--- Test 1 (ENV) ---\n";
    setenv("PROC_TO_KILL", "sleep", 1);
    
    pid_t dummyPid1 = LaunchDummyProcess("sleep", "60");
    sleep(1);

    if (IsProcessRunning(dummyPid1)) 
        std::cout << "[User] Sleep running (PID: " << dummyPid1 << ")" << std::endl;

    system("./Killer");

    waitpid(dummyPid1, NULL, WNOHANG);

    if (!IsProcessRunning(dummyPid1)) 
        std::cout << "[User] SUCCESS: Sleep killed via ENV." << std::endl;
    else 
        std::cerr << "[User] ERROR: Sleep still alive after waitpid." << std::endl;

    unsetenv("PROC_TO_KILL");

    std::cout << "\n--- Test 2 (--name) ---\n";
    pid_t dummyPid2 = LaunchDummyProcess("sleep", "60");
    sleep(1);

    std::string cmd2 = "./Killer --name sleep";
    system(cmd2.c_str());

    waitpid(dummyPid2, NULL, WNOHANG);

    if (!IsProcessRunning(dummyPid2)) 
        std::cout << "[User] SUCCESS: Sleep killed via --name." << std::endl;
    else 
        std::cerr << "[User] ERROR: Sleep still alive after waitpid." << std::endl;

    std::cout << "\n--- Test 3 (--id) ---\n";
    pid_t dummyPid3 = LaunchDummyProcess("sleep", "60");
    sleep(1);
    
    std::string cmd3 = "./Killer --id " + std::to_string(dummyPid3);
    system(cmd3.c_str());

    waitpid(dummyPid3, NULL, WNOHANG);

    if (!IsProcessRunning(dummyPid3)) 
        std::cout << "[User] SUCCESS: Sleep killed via --id." << std::endl;
    else 
        std::cerr << "[User] ERROR: Sleep still alive after waitpid." << std::endl;

    return 0;
}