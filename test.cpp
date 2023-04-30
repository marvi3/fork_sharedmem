#include <unistd.h>
#include <wait.h>
#include <stdint.h>
#include <iostream>
#include <sys/mman.h>
#include "mmap_use.h"
#include "shmget_use.h"

#define PAGESIZE 4096

using namespace std;

void shmget_test() {
    int test = 0;
    pid_t pid = getpid();
    pid = fork();
    if( pid == 0) {
        shmget_use shared_mem(10, 1, "shmem");
        sleep(1);
        cout << "I am a child and this is " << shared_mem.get(0) << endl;
        shared_mem.add(0, "crazy");
        // cout << "Child: " << shared_mem << " - " << shared_mem << endl;
        test += 1;
        cout << "I am a child and this is " << shared_mem.get(0) << endl;
        
    } else {
        shmget_use shared_mem(10, 1, "shmem");
        shared_mem.add(0, "a test");
        cout << "I am a parent and this is " << shared_mem.get(0) << endl;
        
        wait(NULL);
        cout << "I am a parent and this is " << shared_mem.get(0) << endl;
    }
}

void mmap_test() {
    int test = 0;
    char *shared_mem = (char*)shared_mmap(PAGESIZE);
    write_string(shared_mem, (PAGESIZE / sizeof(char)), "a test");
    pid_t pid = getpid();
    pid = fork();
    if( pid == 0) {
        write_string(shared_mem, (PAGESIZE / sizeof(char)), "crazy");
        // cout << "Child: " << shared_mem << " - " << shared_mem << endl;
        test += 1;
        cout << "I am a child and this is " << shared_mem << endl;
        
    } else {
        cout << "I am a parent and this is " << shared_mem << endl;
        
        wait(NULL);
        cout << "I am a parent and this is " << shared_mem << endl;
    }
}

int main(int argc, char**argv) {
    if((char) argc == 1) {
        shmget_test();
    } else {
        mmap_test();
    }
}
