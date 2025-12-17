chess: ChessMain.o ChessGame.o ChessPiece.o ChessBoard.o
	g++ -Wall -g ChessMain.o ChessGame.o ChessPiece.o ChessBoard.o -o chess

ChessMain.o: ChessMain.cpp ChessGame.h
	g++ -Wall -g -c ChessMain.cpp

ChessGame.o: ChessGame.cpp ChessBoard.h
	g++ -Wall -g -c ChessGame.cpp

ChessPiece.o: ChessPiece.cpp ChessGame.h
	g++ -Wall -g -c ChessPiece.cpp

ChessBoard.o: ChessBoard.cpp ChessPiece.h
	g++ -Wall -g -c ChessBoard.cpp

clean:
	rm -f *.o chess
