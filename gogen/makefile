gogen: main.o gogen.o
	g++ -Wall -g main.o gogen.o -o gogen

main.o: main.cpp common.h mask.h gogen.h
	g++ -Wall -g -c main.cpp

gogen.o: gogen.cpp common.h mask.h gogen.h
	g++ -Wall -g -c gogen.cpp
	
clean:
	rm -f *.o gogen