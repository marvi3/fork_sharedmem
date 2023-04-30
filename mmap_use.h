#include <iostream>

using namespace std;

void *shared_mmap(int len);
bool write_string(char *shared_mem, int shared_len, string str);