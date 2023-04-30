CC=g++
CFLAGS=-c -g -Wall -rdynamic -D_DEBUG
LDFLAGS=-g -rdynamic

SOURCES=  \
test.cpp \
fork.cpp \
mmap_use.cpp \

OBJECTS=$(SOURCES:.cpp=.o)

all: test fork mmap_use.cpp

test: test.o $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o build/$@

fork: fork.o $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o build/$@

mmap_use: mmap_use.o $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o build/$@

clean:
	rm -f build/* $(OBJECTS)
