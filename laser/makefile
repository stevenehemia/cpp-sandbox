laser: main.o laser.o
	g++ -Wall -g main.o laser.o -o laser

main.o: main.cpp laser.h
	g++ -Wall -g -c main.cpp

laser.o: laser.cpp laser.h
	g++ -Wall -g -c laser.cpp

clean:
	rm -f *.o laser