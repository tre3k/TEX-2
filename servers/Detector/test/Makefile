all:
	g++ -c main.cpp -o main.o
	g++ -c plx9030c.cpp -o plx9030c.o
	g++ main.o plx9030c.o -o test

clean:
	rm -rf *.o test
