CC=g++

all: calc

run: calc
	./calc
    
test: calc
	python3 test.py

calc: prog.o
	$(CC) -o calc prog.o

prog.o: prog.cpp
	$(CC) -c prog.cpp
    
clean:
	rm -rf *.o my_prog
        
