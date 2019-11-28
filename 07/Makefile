CC=g++ -std=c++14

all: test

test: main.o
	$(CC) -o test main.o

main.o: main.cpp vector.h
	$(CC) -c main.cpp
    
clean:
	rm -rf *.o
