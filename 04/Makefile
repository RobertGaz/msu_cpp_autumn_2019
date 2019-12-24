CC=g++

all: test

test: bigint.o main.o
	$(CC) -o test bigint.o main.o

bigint.o: bigint.cpp
	$(CC) -c bigint.cpp

main.o: main.cpp
	$(CC) -c main.cpp
    
clean:
	rm -rf *.o 
        
