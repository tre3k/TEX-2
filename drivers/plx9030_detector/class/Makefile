CC=g++

all: test

test: main.o plx9030detector.o
	${CC} main.o plx9030detector.o -o test 

main.o: main.cpp
	${CC} main.cpp -c -o main.o

plx9030detector.o: plx9030detector.cpp
	${CC} plx9030detector.cpp -c -o plx9030detector.o


clean:
	rm -rf *.o test
