#include <iostream>
#include <string>
#include <cctype>
#include <memory>
#include "ChessGame.h"
#include "ChessBoard.h"

using std::string;
using std::cout;
using std::endl;


// Forward declaration
class ChessBoard;


/* ============================ ChessGame class ============================ */

/* Constructor for ChessGame object */
ChessGame::ChessGame() {
	
	initState = "8/8/8/8/8/8/8/8 w"; 	// Empty board
	activeTurn = PieceColour::WHITE; 	// Default to White's turn
	whiteCastleKing = false; 			// Default to no castling rights
	whiteCastleQueen = false; 			// Default to no castling rights
	blackCastleKing = false; 			// Default to no castling rights
	blackCastleQueen = false; 			// Default to no castling rights
	enPassant = "-"; 					// Default to no en passant
	halfmoveClock = 0; 					// Default to zero halfmove clock
	fullmoveCount = 1; 					// Default to first fullmove
	whiteKing = nullptr; 				// No white king pointer yet
	blackKing = nullptr; 				// No black king pointer yet
}


/* Load board and game state from a FEN string */
void ChessGame::loadState(string FEN) {

	using std::cerr;

	// Store input FEN to initial state for backup
	initState = FEN;


	/* ------------------ FEN parsing ------------------ */

	// Split FEN into its components
	string parts[6];
	unsigned int cursor = 0;
	int count = 0;
	for(int i = 0; i < 6; i++) {
		parts[i] = "";
		while(cursor < FEN.size() && FEN[cursor] != ' ') {
			parts[i] += FEN[cursor];
			cursor++;
		}
		cursor++;
		if(!parts[i].empty()) count++;
	}


	/* ---------- Error checks of FEN string ----------- */

	// Check for minimum required fields (placement and active turn)
	if(count < 2) {
		// Not enough fields
		cerr << "Invalid FEN: Not enough fields" << endl;
		return;
	}

	// Validate placement field
	int sqCount = 0;
	for(unsigned int i = 0; i < parts[0].size(); i++) {
		char p[13] = {'P','N','B','R','Q','K','p','n','b','r','q','k','/'};
		char ch = parts[0][i];
		// Check character validity
		if(ch < '1' || ch > '8') {
			int chCount = 0;
			while(chCount < 13 && ch != p[chCount]) { chCount++; }
			if(chCount == 13) {
				// Invalid character found
				cerr << "Invalid FEN: Invalid piece character '" << ch
					 << "'" << endl;
				return;
			}
		}
		// Checks for correct number of squares
		if(ch == '/') { continue; }
		else if (ch >= '1' && ch <= '8') { sqCount += parts[0][i] - '0'; }
		else { sqCount++; }
	}
	if(sqCount != 64) {
		// The number of squares do not add up to 64
		cerr << "Invalid FEN: Incorrect number of squares in placement"
			 << endl;
		return;
	}

	// Check for active turn validity
	if(parts[1] != "w" && parts[1] != "b") {
		// Invalid active turn character
		cerr << "Invalid FEN: Active turn must be 'w' or 'b'" << endl;
		return;
	}

	/* --------------- Game state setup ---------------- */

	// Set active turn
	activeTurn = (parts[1] == "w") ?
		PieceColour::WHITE : PieceColour::BLACK;

	// Set castling rights
	if(count >= 2) {
		whiteCastleKing = (parts[2].find('K') != string::npos);
		whiteCastleQueen = (parts[2].find('Q') != string::npos);
		blackCastleKing = (parts[2].find('k') != string::npos);
		blackCastleQueen = (parts[2].find('q') != string::npos);
	}

	// Set en passant target square
	enPassant = (count >= 3) ? parts[3] : "-";

	// Set halfmove clock and fullmove count
	halfmoveClock = (count >= 4) ? std::stoi(parts[4]) : 0;
	fullmoveCount = (count >= 5) ? std::stoi(parts[5]) : 1;

	// Initialise and populate board
	board = ChessBoard();
	whiteKing = nullptr;
	blackKing = nullptr;
	populateBoard(parts[0]);

	cout << "A new board state is loaded!" << endl;

	// Display the board
	// board.display();
}


/* Populate the chessboard based on the placement string */
void ChessGame::populateBoard(string plcm) {
	
	// Start from top-left corner
	char f = 'A', r = '8';
	unsigned int i = 0;

	// Parse placement string, end at space or end of string
	while(i < plcm.size() && plcm[i] != ' ') {
		// Check for '/' indicating new rank
		if(plcm[i] == '/') {
			// Move to next rank
			r--; f = 'A'; i++;
			continue;
		}
		// Check for digit indicating empty squares
		if(plcm[i] >= '1' && plcm[i] <= '8') {
			// Create empty squares
			for(int j = 0; j < plcm[i] - '0'; j++) {
				board[f][r] = nullptr;
				f++;
			}
		} else {
			// Piece character found, create piece
			createPiece(plcm[i], string({f, r}));
			f++;
		}
		i++;
	}
}


/* Create a piece on the board at the given position based on the character */
void ChessGame::createPiece(char p, string pos) {

	// Create piece based on character
	switch(p) {
		case 'P': case 'p':
			board[pos[0]][pos[1]] = std::make_unique<Pawn>
				(isupper(p) ? PieceColour::WHITE : PieceColour::BLACK,
				pos, this);
			break;
		case 'N': case 'n':
			board[pos[0]][pos[1]] = std::make_unique<Knight>
				(isupper(p) ? PieceColour::WHITE : PieceColour::BLACK,
				pos, this);
			break;
		case 'B': case 'b':
			board[pos[0]][pos[1]] = std::make_unique<Bishop>
				(isupper(p) ? PieceColour::WHITE : PieceColour::BLACK,
				pos, this);
			break;
		case 'R': case 'r':
			board[pos[0]][pos[1]] = std::make_unique<Rook>
				(isupper(p) ? PieceColour::WHITE : PieceColour::BLACK,
				pos, this);
			break;
		case 'Q': case 'q':
			board[pos[0]][pos[1]] = std::make_unique<Queen>
				(isupper(p) ? PieceColour::WHITE : PieceColour::BLACK,
				pos, this);
			break;
		case 'K': case 'k':
			board[pos[0]][pos[1]] = std::make_unique<King>
				(isupper(p) ? PieceColour::WHITE : PieceColour::BLACK,
				pos, this);
			// Treat king as special piece and store its pointer
			if(isupper(p)) {
				whiteKing = static_cast<King*>(board[pos[0]][pos[1]].get());
			} else { 
				blackKing = static_cast<King*>(board[pos[0]][pos[1]].get());
			}
			break;
	}
}


/* Evaluate a move and submit it if legal */
void ChessGame::submitMove(string from, string to) {

	/* ------- Preliminary move attempt checks --------- */

	// Check if the inputted positions are valid
	if(!isInputValid(from, to)) {
		cout << "Invalid input!" << endl;
		return;
	}

	// Check if there is a piece at the source square
	if(!isPieceExist(from)) {
		cout << "There is no piece at position " << from << "!" << endl;
		return;
	}
	
	// Check if it is the correct turn
	if(!isRightTurn(from)) {
		cout << "It is not " << board[from[0]][from[1]]->getColour()
			 << "'s turn to move!" << endl;
		return;
	}


	/* ------------- Move legality check --------------- */

	// Evaluate legality and type of move
	MoveType mt = isLegal(from, to);

	if(mt == MoveType::ILLEGAL) {
		// Illegal move
		cout << board[from[0]][from[1]]->getColour() << "'s "
			 << board[from[0]][from[1]]->getType() << " cannot move from "
			 << from << " to " << to << endl;
		 return;
	}


	/* ----------- Castling move execution ------------- */

	if(mt == MoveType::CASTLE) {
        bool kingSide = (to[0] > from[0]);

        char rookFromFile = kingSide ? 'H' : 'A';
        char rookToFile = kingSide ? 'F' : 'D';
        string rookFrom = { rookFromFile, from[1] };
        string rookTo = { rookToFile, from[1] };

        // Move king
        auto king = std::move(board[from[0]][from[1]]);
        auto &kingDest = board[to[0]][to[1]];
        kingDest = std::move(king);
        kingDest->setPosition(to);

        // Move rook
        auto &rookSrc = board[rookFrom[0]][rookFrom[1]];
        auto &rookDst = board[rookTo[0]][rookTo[1]];

        std::unique_ptr<Piece> rook = std::move(rookSrc);
        rookDst = std::move(rook);
        rookDst->setPosition(rookTo);
        rookSrc = nullptr;

		cout << activeTurn << " castles ";
		if(to[0] > from[0]) { cout << "Kingside"; }
		else { cout << "Queenside"; }
		cout << endl;
	}


	/* ------- Standard & capture move execution ------- */

	if(mt == MoveType::STANDARD || mt == MoveType::CAPTURE) {
		// Standard or capture move
        std::unique_ptr<Piece> srcSq = std::move(board[from[0]][from[1]]);
        std::unique_ptr<Piece> destSq = std::move(board[to[0]][to[1]]);

        board[to[0]][to[1]] = std::move(srcSq);
        board[to[0]][to[1]]->setPosition(to);
        board[from[0]][from[1]] = nullptr;

		cout << board[to[0]][to[1]]->getColour() << "'s "
			 << board[to[0]][to[1]]->getType() << " moves from " << from
			 << " to " << to;
		// Additional capture info
		if(mt == MoveType::CAPTURE) {
			cout << " taking " << destSq->getColour() << "'s "
				 << destSq->getType();
		}
	cout << endl;
	}


	/* ---------------- Turn switching ----------------- */

	// Switch active turn
    activeTurn = (activeTurn == PieceColour::WHITE ?
		PieceColour::BLACK : PieceColour::WHITE);


	/* ---------- Post-move game state checks ---------- */

	// Check if opponent's king is checkmated
	if(isCheckmate(activeTurn)) {
		cout << activeTurn << " is in checkmate" << endl;
		// Display the updated board
		// board.display();
		return;
	}

	// Check if opponent is stalemated
	if(isStalemate(activeTurn)) {
		cout << "Draw by stalemate" << endl;
		// Display the updated board
		// board.display();
		return;
	}
	
    // Check if opponent’s king is now in check
	if(isInCheck(activeTurn)) {
		cout << activeTurn << " is in check" << endl;
	}

	// Update game state variables
	updateGameState(mt, to);

	// Display the updated board
	// board.display();
}


/* Update game state after a move */
void ChessGame::updateGameState(MoveType mt, string to) {

	// Calculate move counters
	if(activeTurn == PieceColour::WHITE) fullmoveCount++;
	if(mt == MoveType::CAPTURE ||
	   board[to[0]][to[1]]->getType() == PieceType::PAWN) {
		halfmoveClock = 0;
	} else { halfmoveClock++; }

	// Check for fifty-move rule draw
	if(halfmoveClock >= 100) {
		cout << "Draw by fifty-move rule" << endl;
		return;
	}

	// Update castling rights
    auto &e1 = board['E']['1'];
    if(e1 == nullptr || e1->getType() != PieceType::KING ||
	   e1->getColour() != PieceColour::WHITE) {
		// White king has moved or captured
        whiteCastleKing = false;
        whiteCastleQueen = false;
    }

    auto &e8 = board['E']['8'];
    if(e8 == nullptr || e8->getType() != PieceType::KING ||
	   e8->getColour() != PieceColour::BLACK) {
		// Black king has moved or captured
        blackCastleKing = false;
        blackCastleQueen = false;
    }

    auto &a1 = board['A']['1'];
    if(a1 == nullptr || a1->getType() != PieceType::ROOK ||
	   a1->getColour() != PieceColour::WHITE) {
		// White queenside rook has moved or captured
        whiteCastleQueen = false;
    }

    auto &h1 = board['H']['1'];
    if(h1 == nullptr || h1->getType() != PieceType::ROOK ||
	   h1->getColour() != PieceColour::WHITE) {
		// White kingside rook has moved or captured
        whiteCastleKing = false;
    }

    auto &a8 = board['A']['8'];
    if(a8 == nullptr || a8->getType() != PieceType::ROOK ||
	   a8->getColour() != PieceColour::BLACK) {
		// Black queenside rook has moved or captured
        blackCastleQueen = false;
    }

    auto &h8 = board['H']['8'];
    if(h8 == nullptr || h8->getType() != PieceType::ROOK ||
	   h8->getColour() != PieceColour::BLACK) {
		// Black kingside rook has moved or captured
        blackCastleKing = false;
    }
}


bool ChessGame::isInputValid(string from, string to) const {

	// Check the position consist of 2 characters only
	if(from.size() != 2 || to.size() != 2) { return false; }

	// Check the source position is within chess board bounds
	if(from[0] < 'A' || from[0] > 'H' || from[1] < '1' || from[1] > '8') {
		return false; }

	// Check the target position is within chess board bounds
	if(to[0] < 'A' || to[0] > 'H' || to[1] < '1' || to[1] > '8') {
		return false; }
	
	return true;
}


// Check if there is a piece at the given position
bool ChessGame::isPieceExist(string pos) const{
	
	if(board[pos[0]][pos[1]] == nullptr) {
		return false;
	}
	return true;
}


// Check if it is the correct turn
bool ChessGame::isRightTurn(string pos) const {
	if(board[pos[0]][pos[1]]->getColour() != activeTurn) {
		return false;
	}	
	return true;
}


// Evaluate if a move is pseudo-legal (ignores checks)
MoveType ChessGame::isPseudoLegal(string from, string to) {
	
	// Get piece at source square
	Piece *p = board[from[0]][from[1]].get();


	/* ---------------- Castling branch ---------------- */

	if(p->getType() == PieceType::KING) {

		// Check for kingside castling attempt
		if(to == string({char(from[0] + 2), from[1]})) {
			// Kingside castling attempt
			if(p->getColour() == PieceColour::WHITE) {
				// Check castling rights and intermediary squares
				if(whiteCastleKing &&
				   board['F'][from[1]] == nullptr &&
				   board['G'][from[1]] == nullptr) {
					// Has castling right and path is clear
					return MoveType::CASTLE;
				}
			} else {
				// Check castling rights and intermediary squares
				if(blackCastleKing &&
				   board['F'][from[1]] == nullptr &&
				   board['G'][from[1]] == nullptr) {
					// Has castling right and path is clear
					return MoveType::CASTLE;
				}
			}
		}

		// Check for queenside castling attempt
		if(to == string({char(from[0] - 2), from[1]})) {
			// Queenside castling attempt
			if(p->getColour() == PieceColour::WHITE) {
				// Check castling rights and intermediary squares
				if(whiteCastleQueen &&
				   board['B'][from[1]] == nullptr &&
				   board['C'][from[1]] == nullptr &&
				   board['D'][from[1]] == nullptr) {
					// Has castling right and path is clear
					return MoveType::CASTLE;
				}
			} else {
				// Check castling rights and intermediary squares
				if(blackCastleQueen &&
				   board['B'][from[1]] == nullptr &&
				   board['C'][from[1]] == nullptr &&
				   board['D'][from[1]] == nullptr) {
					// Has castling right and path is clear
					return MoveType::CASTLE;
				}
			}
		}
	}


	/* ------------- Standard/capture branch ----------- */

	// Calculate possible moves and attacks for the piece
	p->calculateMoves();

	// If target square is in piece's attack list -> capture
	for(const auto& at : p->attacks) {
		if(at == to) return MoveType::CAPTURE;
	}

	// If target square is in piece's move list -> standard
	for(const auto& mv : p->moves) {
		if(mv == to) return MoveType::STANDARD;
	}


	/* --------------- Illegal move branch ------------- */

	// Move is not found in either list -> illegal
	return MoveType::ILLEGAL;
}


/* Evaluate if a move is legal (considering check)*/
MoveType ChessGame::isLegal(string from, string to) {

	// Get pseudo-legality & move type evaluation
	MoveType mt = isPseudoLegal(from, to);

	if(mt == MoveType::ILLEGAL) { return MoveType::ILLEGAL; }


	/* ---------------- Castling branch ---------------- */

	if(mt == MoveType::CASTLE) {
		if(!canCastle(from, to)) { return MoveType::ILLEGAL; }

		return MoveType::CASTLE;
	}
	
	
	/* ------------- Standard/capture branch ----------- */
	
	auto& targetSq = board[to[0]][to[1]];
   	auto& movingPiece = board[from[0]][from[1]];

    std::unique_ptr<Piece> backupTarget = std::move(targetSq);
   	std::unique_ptr<Piece> backupPiece = std::move(movingPiece);

   	// Simulate move
   	targetSq = std::move(backupPiece);
   	targetSq->setPosition(to);
   	movingPiece = nullptr;

	bool illegal = isInCheck(activeTurn);

	// Undo move
	movingPiece = std::move(targetSq);
	movingPiece->setPosition(from);
	targetSq = std::move(backupTarget);

	if(illegal) return MoveType::ILLEGAL;
	
	return mt;
}


/* Evaluate if a position is in check for a given colour */
bool ChessGame::isInCheck(PieceColour clr) const {

	// Get king position and opponent's colour
	Piece* king = (clr == PieceColour::WHITE) ? whiteKing : blackKing;
	string kingPos = king->getPosition();
	PieceColour oppColour = (clr == PieceColour::WHITE) ?
		PieceColour::BLACK : PieceColour::WHITE;

	// Return true if king's position is attacked by any opponent piece
	// false otherwise
	return isSquareAttacked(kingPos, oppColour);
}


/* Evaluate if a given color is in checkmate */
bool ChessGame::isCheckmate(PieceColour clr) {

	// Checkmate occurs if in check and no legal moves available
	if(isInCheck(clr) && !hasLegalMoves(clr)) { return true; }

	return false;
}


/* Evaluate if a given color is in stalemate */
bool ChessGame::isStalemate(PieceColour clr) {

	// Stalemate occurs if not in check and no legal moves available
	if(!isInCheck(clr) && !hasLegalMoves(clr)) { return true; }

	return false;
}


/* Evaluate if a given color has any legal moves */
bool ChessGame::hasLegalMoves(PieceColour clr) {

	/* ---------- Standard & capture branch ----------- */

	// Scan board for pieces of the given colour
	for(char f = 'A'; f <= 'H'; f++) {
		for(char r = '1'; r <= '8'; r++) {

			// Get reference to square
			auto &sq = board[f][r];

			// Skip empty squares or opponent's pieces
			if(sq == nullptr || sq->getColour() != clr) continue;

			// Calculate piece's possible moves and attacks
			sq->calculateMoves();

			// Iterate through all possible moves
			for(const auto &to : sq->moves) {
				// Check if move is legal
				if(isLegal(sq->getPosition(), to) != MoveType::ILLEGAL) {
					return true;
				}
			}

			// Iterate through all possible attacks
			for(const auto &to : sq->attacks) {
				// Check if attack is legal
				if(isLegal(sq->getPosition(), to) != MoveType::ILLEGAL) {
					return true;
				}
			}
		}
	}


	/* ---------------- Castling branch ---------------- */

	// If in check, castling is illegal
    if (!isInCheck(clr)) {

		// Get king position
	    Piece* king = (clr == PieceColour::WHITE ? whiteKing : blackKing);
    	string kp = king->getPosition();
		
		// Get castling target squares
        string kside = { char(kp[0] + 2), kp[1] };
        string qside = { char(kp[0] - 2), kp[1] };

		// Check legality of castling moves
        if (isLegal(kp, kside) == MoveType::CASTLE) return true;
        if (isLegal(kp, qside) == MoveType::CASTLE) return true;
    }

	// No legal moves found
	return false;
}


/* Evaluate if a square is attacked by any piece of the given colour */
bool ChessGame::isSquareAttacked(string pos, PieceColour byColour) const {
	// Scan board for pieces of the given colour
	for(char f = 'A'; f <= 'H' ; f++) {
		for(char r = '1'; r <= '8'; r++) {

			auto &sq = board[f][r];

			// Skip empty squares or own pieces
			if(sq == nullptr || sq->getColour() != byColour) continue;

			// Calculate opponent piece's moves and attacks
			sq->calculateMoves();

			for(const auto& attack : sq->attacks) {
				// Check if opponent piece attacks the given position
				if(attack == pos) return true;
			}

			for(const auto& move : sq->moves) {
				// Check if opponent piece can move to the given position
				// Pawns cannot attack by moving
				if(sq->getType() == PieceType::PAWN) continue; 
				if(move == pos) return true;
			}
		}
	}
	return false;
}


/* Evaluate if castling is possible */
bool ChessGame::canCastle(const string& from, const string& to) const {

	// Determine own and opponent colours
	PieceColour ownColour = activeTurn;
	PieceColour oppColour = (ownColour == PieceColour::WHITE) ?
		PieceColour::BLACK : PieceColour::WHITE;

	// to[0] > from[0] -> kingside castle, else queenside
	string mid;
	if(to[0] > from[0]) {
		// Middle square for kingside
		mid = string({char(from[0] + 1), from[1]});
	} else {
		// Middle square for queenside
		mid = string({char(from[0] - 1), from[1]});
	}

	// Illegal to castle if the king is in check
	if(isSquareAttacked(from, oppColour)) return false;

	// Illegal to castle if intermediary square is under attack
	if(isSquareAttacked(mid, oppColour)) return false;

	// Illegal to castle if destination square is under attack
	if(isSquareAttacked(to, oppColour)) return false;

	return true;
}


/* Get piece at the given position */
Piece* ChessGame::getPiece(string pos) const {
	return board.getPieceAt(pos);
}


/* Set piece at the given position */
void ChessGame::setPiece(string pos, std::unique_ptr<Piece> p) {
	board.setPieceAt(pos, std::move(p));
}


/* Remove piece from the given position */
std::unique_ptr<Piece> ChessGame::removePiece(string pos) {
	return board.removePieceAt(pos);
}


/* Display the current board state */
void ChessGame::display() const { board.display(); }
