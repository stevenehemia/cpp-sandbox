#include <ostream>
#include <string>
#include <cctype>
#include "ChessPiece.h"
#include "ChessGame.h"
#include "ChessBoard.h"

using std::string;


/* =========================== PieceColour class =========================== */

/* Override << operator to output PieceColour */
std::ostream& operator<<(std::ostream& os, PieceColour colour) {
    switch(colour) {
        case PieceColour::WHITE: os << "White"; break;
        case PieceColour::BLACK: os << "Black"; break;
    }
    return os;
}


/* ============================ PieceType class ============================ */

/* Override << operator to output PieceColour */
std::ostream& operator<<(std::ostream& os, PieceType type) {
    switch(type) {
		case PieceType::PAWN: os << "Pawn"; break;
		case PieceType::KNIGHT: os << "Knight"; break;
		case PieceType::BISHOP: os << "Bishop"; break;
		case PieceType::ROOK: os << "Rook"; break;
		case PieceType::QUEEN: os << "Queen"; break;
		case PieceType::KING: os << "King"; break;
    }
    return os;
}


/* ============================== Piece class ============================== */

/* Constructor for (abstract) Piece object */
Piece::Piece(PieceColour c, string p, ChessGame* g)
	: colour(c), position(p), game(g) {}


/* Helper function to check if a position is within the board bounds */
bool Piece::isInBound(string pos) const {
	if(pos[0] < 'A' || pos[0] > 'H' || pos[1] < '1' || pos[1] > '8') {
		return false;
	}
	return true;
}


/* Helper function to check if a position is blocked by any piece */
bool Piece::isBlocked(string pos) const {
	if(game->getPiece(pos) != nullptr) { return true; }
	return false;
}


/* Generate possible moves and attacks along the given directions */
void Piece::addSlidingMoves(const int dir[4][2]) {

	// Get piece's current file and rank
	int pf = position[0], pr = position[1];

	// Iterate over each direction
	for(int i = 0; i < 4; i++) {
		// Extend moves in the current direction
		for(int j = 1; ; j++) {
			
			// Calculate target square
			string sq = string({char(pf + j * dir[i][0]),
								char(pr + j * dir[i][1])});

			if(!isInBound(sq)) { break; }  // Out of bounds, change direction

			if(isBlocked(sq)) {
				// Square blocked, add to attack list if opponent piece
				if(game->getPiece(sq)->getColour() != colour) attacks.push_back(sq);
				
				break;  // Change direction
			}

		moves.push_back(sq);  // Square is free, add to move list
		}
	}
}


/* Generate possible moves and attacks based on given the offsets */
void Piece::addOffsetMoves(const int ofs[8][2]) {

	// Get piece's current file and rank
	int pf = position[0], pr = position[1];

	// Iterate over each offset
	for (int i = 0; i < 8; i++) {

		// Calculate target square
		string sq = string({char(pf + ofs[i][0]),
							char(pr + ofs[i][1])});

		if(!isInBound(sq)) { continue; }  // Out of bounds, next offset

		if(isBlocked(sq)) {
			// Square blocked, add to attack list if opponent piece
			if(game->getPiece(sq)->getColour() != colour) attacks.push_back(sq);
			
			continue;  // Next offset
		}

		moves.push_back(sq);  // Square is free, add to move list
	}
}


/* Calculate possible moves for a Piece */
void Piece::calculateMoves() {

	moves.clear();		// Clear previous moves
	attacks.clear();	// Clear previous attacks

	// Generate moves based on piece type
	switch(type) {
		case PieceType::KING: addOffsetMoves(ADJACENT); break;
		case PieceType::QUEEN:
			addSlidingMoves(STRAIGHTS);
			addSlidingMoves(DIAGONALS);
			break;
		case PieceType::ROOK: addSlidingMoves(STRAIGHTS); break;
		case PieceType::BISHOP: addSlidingMoves(DIAGONALS); break;
		case PieceType::KNIGHT: addOffsetMoves(L_SHAPE); break;
		case PieceType::PAWN: break; // Pawn has its own calculateMoves()
		// Throw error for unknown piece type
		default: std::cerr << "Error: Unknown piece type" << std::endl; break;
	}
}


/* Get symbol representing the piece */
char Piece::getSymbol() const {

	switch(type) {
		case PieceType::PAWN:
			return (colour == PieceColour::WHITE) ? 'P' : 'p';
		case PieceType::KNIGHT:
			return (colour == PieceColour::WHITE) ? 'N' : 'n';
		case PieceType::BISHOP:
			return (colour == PieceColour::WHITE) ? 'B' : 'b';
		case PieceType::ROOK:
			return (colour == PieceColour::WHITE) ? 'R' : 'r';
		case PieceType::QUEEN:
			return (colour == PieceColour::WHITE) ? 'Q' : 'q';
		case PieceType::KING:
			return (colour == PieceColour::WHITE) ? 'K' : 'k';
	}
	// Throw error for unknown piece type
	std::cerr << "Error: Unknown piece type" << std::endl;
	return '?';
}


/* Get the type of the piece */
PieceType Piece::getType() const { return type; }

/* Get the colour of the piece */
PieceColour Piece::getColour() const { return colour; }

/* Get the position of the piece */
string Piece::getPosition() const { return position; }

/* Set the position of the piece */
void Piece::setPosition(const string& newPos) {	position = newPos; }


/* ============================== Pawn class =============================== */

/* Constructor for Pawn object */
Pawn::Pawn(PieceColour c, string p, ChessGame* g)
	: Piece(c, p, g) { type = PieceType::PAWN; }


/* Calculate possible moves for a Pawn */
void Pawn::calculateMoves() {

	moves.clear();		// Clear previous moves
	attacks.clear();	// Clear previous attacks

	// Get piece's current file and rank
	int pf = position[0], pr = position[1];

	// Determine movement direction based on colour
	int dir = (colour == PieceColour::WHITE) ? 1 : -1;

	
	/* --------------- Move calculation ---------------- */
	
	// Single square forward
	string sq = string({char(pf), char(pr + dir)});

	if(!isBlocked(sq) && isInBound(sq)) {
		// Square is free, add to move list
		moves.push_back(sq);

		if((colour == PieceColour::WHITE && pr == '2') ||
		   (colour == PieceColour::BLACK && pr == '7')) {

			// Pawn is on starting rank, check two squares forward
			sq = string({char(pf), char(pr + 2*dir)});

			// If two squares ahead is free, add to move list
			if(!isBlocked(sq) && isInBound(sq)) { moves.push_back(sq); }
		}
	}


	/* -------------- Attack calculation --------------- */

	// Alt represents the adjacent files
	for(int alt : {1, -1}) {
		// Calculate target square (forward-diagonal)
		string sq = string({char(pf + alt), char(pr + dir)});

		if(isBlocked(sq) && isInBound(sq)) {
			if(game->getPiece(sq)->getColour() != colour) {
				// Opponent piece found, add to attack list
				attacks.push_back(sq);
			}
		}
	}
}


/* ============================= Knight class ============================== */

/* Constructor for Knight object */
Knight::Knight(PieceColour c, string p, ChessGame* g)
	: Piece (c, p, g) { type = PieceType::KNIGHT; }


/* ============================= Bishop class ============================== */

/* Constructor for Bishop object */
Bishop::Bishop(PieceColour c, string p, ChessGame* g)
	: Piece(c, p, g) { type = PieceType::BISHOP; }


/* ============================== Rook class =============================== */

/* Constructor for Rook object */
Rook::Rook(PieceColour c, string p, ChessGame* g)
	: Piece (c, p, g) { type = PieceType::ROOK; }


/* ============================= Queen class =============================== */

/* Constructor for Queen object */
Queen::Queen(PieceColour c, string p, ChessGame* g)
	: Piece(c, p, g) { type = PieceType::QUEEN; }


/* ============================= King class ================================ */

/* Constructor for King object */
King::King(PieceColour c, string p, ChessGame* g)
	: Piece(c, p, g) { type = PieceType::KING; }
