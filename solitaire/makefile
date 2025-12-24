solitaire: main.o solitaire.o
	g++ -Wall -g main.o solitaire.o -o solitaire

main.o: main.cpp solitaire.h
	g++ -Wall -g -c main.cpp

solitaire.o: solitaire.cpp solitaire.h
	g++ -Wall -g -c solitaire.cpp

clean:
	rm -f *.o solitaire