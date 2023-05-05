g++ -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux sharedMem.cpp -o sharedMem.o
g++ -shared -fPIC -o libnative.so sharedMem.o -lc
java -cp . -Djava.library.path=/home/kali/Desktop/Masterarbeit/fork_sharedmem/java sharedMem 