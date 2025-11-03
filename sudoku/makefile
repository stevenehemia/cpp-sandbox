sudoku: main.o sudoku.o
	g++ main.o sudoku.o -o sudoku

main.o: main.o sudoku.h
	g++ -c main.cpp sudoku.h

sudoku.o: sudoku.o sudoku.h
	g++ -c sudoku.cpp sudoku.h