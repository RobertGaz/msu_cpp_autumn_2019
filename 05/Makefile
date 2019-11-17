CC=g++ -std=c++11

all: test

test: serializer.o main.o
	$(CC) -o test serializer.o main.o

serializer.o: serializer.cpp
	$(CC) -c serializer.cpp

main.o: main.cpp
	$(CC) -c main.cpp
    
clean:
	rm -rf *.o
