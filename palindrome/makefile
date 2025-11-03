words: main.o words.o
	g++ -Wall -g main.o words.o -o words

main.o: main.cpp words.h
	g++ -Wall -g -c main.cpp words.h

words.o: words.cpp words.h
	g++ -Wall -g -c words.cpp words.h

clean:
	rm -f *.o execute