CC=g++ -std=c++11 -pthread

all: test

test: thread_pool.o main.o
	$(CC) -o test thread_pool.o main.o

thread_pool.o: thread_pool.cpp thread_pool.h
	$(CC) -c thread_pool.cpp

main.o: main.cpp
	$(CC) -c main.cpp
    
clean:
	rm -rf *.o
