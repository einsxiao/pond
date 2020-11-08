FLAGS := -std=c++11 -O2 

all: test.o
	g++ $(FLAGS) -o test test.o `pond-path cpp` -lpd
	./test

test.o: test.cpp
	g++ $(FLAGS) -c test.cpp `pond-path cpp`



