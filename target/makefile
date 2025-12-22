target: main.o target.o
	g++ -Wall -g main.o target.o -o target

main.o: main.cpp target.h
	g++ -Wall -g -c main.cpp

target.o: target.cpp target.h
	g++ -Wall -g -c target.cpp

clean:
	rm -f *.o target