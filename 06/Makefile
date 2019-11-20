CC=g++ -std=c++11

all: test

test: format.o main.o
	$(CC) -o test format.o main.o

format.o: format.cpp
	$(CC) -c format.cpp

main.o: main.cpp
	$(CC) -c main.cpp
    
clean:
	rm -rf *.o
