#include <iostream>
#include <string>
#include <memory>
#include "ChessBoard.h"

using std::string;


/* ========================= ChessBoard class ========================= */

/* Overload [] operator to accept chess file and rank notation to access
   board squares */
ChessBoard::RankAccessor ChessBoard::operator[](char fileChar) {
	return RankAccessor{fileChar - 'A', this};
}
const ChessBoard::RankAccessor ChessBoard::operator[](char fileChar) const {
	return RankAccessor{fileChar - 'A', const_cast<ChessBoard*>(this)};
}
std::unique_ptr<Piece>& ChessBoard::RankAccessor::operator[](char rankChar) {
	return board->grid[rankChar - '1'][file];
}
const std::unique_ptr<Piece>& ChessBoard::RankAccessor::operator[]
(char rankChar) const {
	return board->grid[rankChar - '1'][file];
}


/* Constructor for ChessBoard object */
ChessBoard::ChessBoard() {

	// Initialize all squares to nullptr
	for(int r = 0; r < 8; r++) {
		for(int c = 0; c < 8; c++) {
			grid[r][c] = nullptr;
		}
	}
}


/* Get the piece at the given position */
Piece* ChessBoard::getPieceAt(string pos) const {
	if(pos[0] < 'A' || pos[0] > 'H' || pos[1] < '1' || pos[1] > '8')
		return nullptr;
	return grid[pos[1]-'1'][pos[0]-'A'].get();
}


/* Set the piece at the given position */
void ChessBoard::setPieceAt(string pos, std::unique_ptr<Piece> p) {
	grid[pos[1]-'1'][pos[0]-'A'] = std::move(p);
}


/* Remove and return the piece at the given position */
std::unique_ptr<Piece> ChessBoard::removePieceAt(string pos) {
    auto tmp = std::move(grid[pos[1]-'1'][pos[0]-'A']);
    grid[pos[1]-'1'][pos[0]-'A'] = nullptr;
    return tmp;
}


/* Display the current state of the chessboard */
void ChessBoard::display() const {

	using std::cout;

	cout << "  +---+---+---+---+---+---+---+---+\n";
	for(int r = 7; r >= 0; r--) {
		cout << r + 1 << " | ";
		for(int c = 0; c < 8; c++) {
			if(grid[r][c] == nullptr) {
				cout << "  | ";
			} else {
				cout << grid[r][c]->getSymbol() << " | ";
			}
		}
		cout << "\n  +---+---+---+---+---+---+---+---+\n";
	}
	cout << "    A   B   C   D   E   F   G   H  \n";
}

