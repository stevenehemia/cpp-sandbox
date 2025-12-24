sokoban: main.o sokoban.o
	g++ -Wall -g main.o sokoban.o -o sokoban

main.o: main.cpp sokoban.h
	g++ -Wall -g -c main.cpp

sokoban.o: sokoban.cpp sokoban.h
	g++ -Wall -g -c sokoban.cpp

clean:
	rm -f *.o sokoban