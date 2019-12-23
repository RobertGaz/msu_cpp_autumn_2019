CC0=g++ -std=c++11
CC=g++ -std=c++11 -pthread

all: test

test: create binary_file_sort check
	./create 
	./binary_file_sort 
	./check

binary_file_sort: binary_file_sort.cpp
	$(CC) -o binary_file_sort binary_file_sort.cpp

check: check.cpp
	$(CC0) -o check check.cpp

create: create.cpp
	$(CC0) -o create create.cpp

    
clean:
	rm -rf *.o
	rm -rf *.bin
