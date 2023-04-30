#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <vector>

#include "shmget_use.h"

using namespace std;

//This constructor creates a shared memory with "arrayLen" strings, each with "numChar" chars
shmget_use::shmget_use(int numChar, int arrayLen, char* filename) {
    numElem = arrayLen;
    size = numChar;
    // ftok to generate unique key
    key = ftok(filename,65);
    if(key == IPC_RESULT_ERROR) {
        throw runtime_error("Could not get the key to the shared memory.");
    }

    // shmget returns an identifier in shmid
    shmid = shmget(key,size * numElem,0666|IPC_CREAT);
    // shmat to attach to shared memory
    shared_mem = (char*) shmat(shmid,NULL,0);
}

shmget_use::~shmget_use() {
    shmctl(shmid, IPC_RMID, NULL);
}

bool shmget_use::detach(int i){
    //detach from shared memory 
    return (shmdt(shared_mem) != IPC_RESULT_ERROR);
}

bool shmget_use::add(int index, string str) {
    if(index >= numElem || str.length() >= size) {
        return false;
    }
    string test = "west";
    for (int i = (index * size); i < (index * size) + str.length(); i++) {
        try {
            shared_mem[i] = str[i];
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    for (int i = (index * size) + str.length(); i < ((index + 1) * size); i++) {
        shared_mem[i] = NULL;
    }
    return true;
}

string shmget_use::get(int index) {
    if(index >= numElem) {
        return NULL;
    }
    string returnString = "";
    for (int i = (index * size); i < ((index + 1) * size); i++) {
        if(shared_mem[i] == NULL) {
            return returnString;
        }
        returnString += shared_mem[i];
    }
    return returnString;
    
}

// int shm_init()
// {
//     printf("Data read from memory: %s\n",str);
    
//     // destroy the shared memory
//     shmctl(shmid,IPC_RMID,NULL);
     
//     return 0;
// }