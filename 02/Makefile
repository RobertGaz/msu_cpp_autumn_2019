CC=g++ -std=c++14

all: test

test: LinearAllocator.o main.o
	$(CC) -o test LinearAllocator.o main.o

LinearAllocator.o: LinearAllocator.cpp LinearAllocator.h
	$(CC) -c LinearAllocator.cpp

main.o: main.cpp
	$(CC) -c main.cpp
    
clean:
	rm -rf *.o 
        
