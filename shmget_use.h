#include <iostream>

using namespace std;

class shmget_use {
    key_t key;
    int shmid;
    char* shared_mem;
    int numElem;
    int size;
    int IPC_RESULT_ERROR = -1;
    
public:
    shmget_use(int size, int arrayLen, char* filename);
    ~shmget_use();
    bool detach(int i);
    bool add(int index, string str);
    string get(int index);
};