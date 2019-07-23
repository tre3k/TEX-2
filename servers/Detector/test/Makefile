CC=g++

all: test

test: main.o tdc.o plx9030c.o
	${CC} main.o tdc.o plx9030c.o -o test

main.o: main.cpp
	${CC} -c main.cpp -o main.o

tdc.o: tdc.cpp
	${CC} -c tdc.cpp -o tdc.o

plx9030c.o: plx9030c.cpp
	${CC} -c plx9030c.cpp -o plx9030c.o

clean:
	rm -rf *.o test
