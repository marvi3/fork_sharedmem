#include "sharedMem.h"
#include <unistd.h>
#include <wait.h>
#include <stdint.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <vector>

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

static shmget_use memory;

void testWrite(shmget_use shared_mem, int pos) {
    string input;
    cout << "What do you want to write in the shared memory?" << endl;
    cin >> input;
    shared_mem.add(pos, input);
}

void test() {
    shmget_use shared_mem(10, 1, "../shmem");
    sleep(1);
    while (true) {
      cout << "In Java I read \""<< shared_mem.get(0) << "\"." << endl;
      string input;
      cin >> input;
      if(input == "exit") {
          break;
      } else if (input == "write") {
          testWrite(shared_mem, 0);
      }
      
    }
    // shared_mem.add(0, "crazy");
}

JNIEXPORT void JNICALL Java_sharedMem_add
  (JNIEnv* env, jobject thisObject, jint pos, jstring data) {
    try {
        memory.add(pos, data);
    } catch {
        
    }
}

JNIEXPORT jstring JNICALL Java_sharedMem_get
  (JNIEnv* env, jobject thisObject, jint pos) {
    try {
        jstring ret = memory.get(pos);
        return ret;
    } catch {
        return NULL;
    }
}

JNIEXPORT jboolean JNICALL Java_sharedMem_create
  (JNIEnv* env, jobject, jint numChar, jint arrayLength, jstring fileName) {
    try {
        memory = new shmget_use(numChar, arrayLength, fileName);
        return true;
    } catch {
        return false;
    }
}