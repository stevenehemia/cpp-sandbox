soundex: main.o soundex.o
	g++ -Wall -g main.o. soundex.o -o soundex

main.o: main.cpp soundex.h
	g++ -Wall -g -c main.cpp

soundex.o: soundex.cpp soundex.h
	g++ -Wall -g -c soundex.cpp

clean:
	del *.o soundex.exe