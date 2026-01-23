#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main() {
    // Parent should create 5 children
    for (int i = 0; i <= 4; ++i) {
        pid_t pid = fork();
        
        if (pid < 0) {
            std::cerr << "Fork failed" << std::endl;
            return 1;
        } else if (pid == 0) {
            // Child process
            // Requirement: Display the last child first to first child last
            // We use sleep (WAIT(delay)) to control the order of execution/printing
            // Child 5 (last) waits the least, Child 1 (first) waits the most.
            sleep(5 - i); 
            
            // GETPID of every child
            std::cout << "Child " << i << " Process ID: " << getpid() << std::endl;
            exit(0);
        }
    }

    // Parent waits for all children to finish
    for (int i = 0; i < 5; ++i) {
        wait(NULL);
    }

    std::cout << "Parent Process ID: " << getpid() << std::endl;

    return 0;
}