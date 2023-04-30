#include <unistd.h>
#include <wait.h>
#include <stdint.h>
#include <iostream>
#include <sys/mman.h>

#include "mmap_use.h"

using namespace std;



void *shared_mmap(int len) {
    void *shared_mem = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1 /* fd */, 0 /* offset */);
    if (shared_mem == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }
    return shared_mem;
}

bool write_string(char *shared_mem, int shared_len, string str) {    
    for (int i = 0; i < str.length(); i++) {
        try {
            shared_mem[i] = str[i];
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    if (str.length() < shared_len) {
        for (int i = str.length(); i < shared_len; i++) {
            shared_mem[i] = NULL;
        }
    }
    return true;
}