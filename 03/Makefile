CC=g++

all: test

test: matrix.o main.o
	$(CC) -o test matrix.o main.o

matrix.o: matrix.cpp
	$(CC) -c matrix.cpp

main.o: main.cpp
	$(CC) -c main.cpp
    
clean:
	rm -rf *.o 
        
