#include <unistd.h>
#include <wait.h>
#include <stdint.h>
#include <iostream>
#include <sys/mman.h>
#include "mmap_use.h"

using namespace std;

pair<void *, pid_t> mmap_fork(int len) {
    pair<void *, pid_t> result;
    result.first = shared_mmap(len);
    result.second = fork(); 
    return result;
}

void *fork_loop(int numForks, int len) {
    void *shared_mem_pointer[numForks];
    pid_t pid;
    for (int i = 0; i < numForks; i++) {
        pair<void *, pid_t> result = mmap_fork(len);
        pid = result.second;

        // decide if continue as child or parent process
        if (pid == 0) {
            // beginn child execution
            return result.first;
        } else if(pid > 0) {
            // beginn parent execution
            shared_mem_pointer[i] = result.first;
        } else {
            printf("Error creating the Process. The resulting PID is %d.\n",pid);
        }
    }
    return NULL;
}