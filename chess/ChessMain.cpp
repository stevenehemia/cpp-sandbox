#include"ChessGame.h"

#include<iostream>

using std::cout;

int main() {

	cout << "========================\n";
	cout << "Testing the Chess Engine\n";
	cout << "========================\n\n";

	ChessGame cg;
	cg.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

	cg.submitMove("D7", "D6");
	cout << '\n';

	cg.submitMove("D4", "H6");
	cout << '\n';

	cg.submitMove("D2", "D4");
	cout << '\n';

	cg.submitMove("F8", "B4");
	cout << '\n';

	cout << "=========================\n";
	cout << "Alekhine vs. Vasic (1931)\n";
	cout << "=========================\n\n";

	cg.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

	cg.submitMove("E2", "E4");
	cg.submitMove("E7", "E6");
	cout << '\n';

	cg.submitMove("D2", "D4");
	cg.submitMove("D7", "D5");
	cout << '\n';

	cg.submitMove("B1", "C3");
	cg.submitMove("F8", "B4");
	cout << '\n';

	cg.submitMove("F1", "D3");
	cg.submitMove("B4", "C3");
	cout << '\n';

	cg.submitMove("B2", "C3");
	cg.submitMove("H7", "H6");
	cout << '\n';

	cg.submitMove("C1", "A3");
	cg.submitMove("B8", "D7");
	cout << '\n';

	cg.submitMove("D1", "E2");
	cg.submitMove("D5", "E4");
	cout << '\n';

	cg.submitMove("D3", "E4");
	cg.submitMove("G8", "F6");
	cout << '\n';

	cg.submitMove("E4", "D3");
	cg.submitMove("B7", "B6");
	cout << '\n';

	cg.submitMove("E2", "E6");
	cg.submitMove("F7", "E6");
	cout << '\n';

	cg.submitMove("D3", "G6");
	cout << '\n';

    cout << "========================\n";
    cout << "Stalemate Test Position\n";
    cout << "========================\n\n";

    // Start from standard initial position
    cg.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
    cout << '\n';

    // 1. e3 a5
    cg.submitMove("E2", "E3"); cout << '\n';
    cg.submitMove("A7", "A5"); cout << '\n';

    // 2. Qh5 Ra6
    cg.submitMove("D1", "H5"); cout << '\n';
    cg.submitMove("A8", "A6"); cout << '\n';

    // 3. Qxa5 h5
    cg.submitMove("H5", "A5"); cout << '\n';
    cg.submitMove("H7", "H5"); cout << '\n';

    // 4. h4 Rah6
    cg.submitMove("H2", "H4"); cout << '\n';
    cg.submitMove("A6", "H6"); cout << '\n';

    // 5. Qxc7 f6
    cg.submitMove("A5", "C7"); cout << '\n';
    cg.submitMove("F7", "F6"); cout << '\n';

    // 6. Qxd7+ Kf7
    cg.submitMove("C7", "D7"); cout << '\n';
    cg.submitMove("E8", "F7"); cout << '\n';

    // 7. Qxb7 Qd3
    cg.submitMove("D7", "B7"); cout << '\n';
    cg.submitMove("D8", "D3"); cout << '\n';

    // 8. Qxb8 Qh7
    cg.submitMove("B7", "B8"); cout << '\n';
    cg.submitMove("D3", "H7"); cout << '\n';

    // 9. Qxc8 Kg6
    cg.submitMove("B8", "C8"); cout << '\n';
    cg.submitMove("F7", "G6"); cout << '\n';

    // 10. Qe6 (this is stalemate)
    cg.submitMove("C8", "E6"); cout << '\n';

	cout << "========================\n";
	cout << "Successful Castling Test\n";
	cout << "========================\n\n";

	cg.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

	// 1. e4
	cg.submitMove("E2", "E4"); cout << '\n';
	// ... d5
	cg.submitMove("D7", "D5"); cout << '\n';

	// 2. Nf3
	cg.submitMove("G1", "F3"); cout << '\n';
	// ... Nc6
	cg.submitMove("B8", "C6"); cout << '\n';

	// 3. Bc4
	cg.submitMove("F1", "C4"); cout << '\n';
	// ... Bg4
	cg.submitMove("C8", "G4"); cout << '\n';

	// 4. O-O  (White castles kingside)
	cg.submitMove("E1", "G1"); cout << '\n';
	// ... Qd7
	cg.submitMove("D8", "D7"); cout << '\n';

	// 5. Re1
	cg.submitMove("F1", "E1"); cout << '\n';
	// ... O-O-O  (Black castles queenside)
	cg.submitMove("E8", "C8"); cout << '\n';

	// 6. Nc3
	cg.submitMove("B1", "C3"); cout << '\n';
	// ... e5
	cg.submitMove("E7", "E5"); cout << '\n';

	cout << "========================\n";
	cout << "Illegal Castling Test\n";
	cout << "========================\n\n";

	cg.loadState("4k3/8/8/8/8/8/4r3/4K2R w K - 0 1");
	cout << '\n';

	// Castling while in check - illegal
	cg.submitMove("E1", "G1");
	cout << '\n';

	cg.loadState("r3k2r/8/8/8/8/8/4Q3/4K3 b kq - 0 1");
	cout << '\n';

	// Castling while through attacked square - illegal
	cg.submitMove("E8", "C8");
	cout << '\n';

	cg.loadState("4k3/8/8/8/8/8/6r1/R3K2R w KQ - 0 1");
	cout << '\n';

	// Castling into attacked square - illegal
	cg.submitMove("E1", "G1");
	cout << '\n';

	cg.loadState("4k3/8/8/8/8/8/8/R2BK2R w KQ - 0 1");
	cout << '\n';

	// Castling with pieces in between - illegal
	cg.submitMove("E1", "C1");
	cout << '\n';

	cg.loadState("4k3/8/8/8/8/8/8/R3K2R w K - 0 1");
	cout << '\n';

	// Castling when rook has moved - illegal
	cg.submitMove("E1", "C1");
	cout << '\n';

	cout << "========================\n";
	cout << "Edge Cases Tests\n";
	cout << "========================\n\n";

	cg.loadState("4k3/8/8/8/8/8/8/Q2PK3 w - - 0 1");
	cout << '\n';

	// Queen jumping over pieces horizontally - illegal
	cg.submitMove("A1", "H1");
	cout << '\n';

	cg.loadState("4k3/8/8/8/8/4P3/8/4K3 w - - 0 1");
	cout << '\n';

	// Pawn diagonal capture to empty square - illegal
	cg.submitMove("E3", "F4");
	cout << '\n';

	cg.loadState("4k3/8/4P3/8/8/8/8/4KR2 w - - 0 1");
	cout << '\n';

	// Rook moving to the last rank, checking king - llegal
	cg.submitMove("F1", "F8");
	cout << '\n';

	cg.loadState("4k3/8/8/8/8/8/8/4K3 w - - 0 1");
	cout << '\n';

	// King moving two squares without castling - illegal
	cg.submitMove("E1", "G1");
	cout << '\n';

	cg.loadState("6k1/5ppp/8/8/8/8/8/4R2K w - - 0 1");
	cout << '\n';
	
	// Checkmate in one move - legal
	cg.submitMove("H1", "H8");
	cout << '\n';

	cg.loadState("4k3/8/8/8/3P4/8/8/Q3K3 w - - 0 1");
	cout << '\n';

	// Queen diagonal jump over piece - illegal
	cg.submitMove("A1", "F6");
	cout << '\n';

	cg.loadState("4k3/8/8/8/4p3/4P3/8/4K3 w - - 0 1");
	cout << '\n';

	// Pawn blocked by enemy pawn - illegal
	cg.submitMove("E3", "E4");
	cout << '\n';

	cg.loadState("4k3/8/8/8/3PPP2/3PNP2/3PPP2/4K3 w - - 0 1");
	cout << '\n';

	// Knight blocked by own pieces (still can jump) - legal
	cg.submitMove("E3", "F5");
	cout << '\n';

	cg.loadState("4k3/8/8/8/2B1B3/8/8/4K3 w - - 0 1");
	cout << '\n';

	// Bishop moves up straight - illegal
	cg.submitMove("C4", "H7");
	cout << '\n';

	cg.loadState("4k3/8/8/8/4r3/8/4N3/4K3 w - - 0 1");
	cout << '\n';

	// Moving pinned knight - illegal
	cg.submitMove("E2", "G3");
	cout << '\n';

	cg.loadState("4k3/8/8/8/4r3/8/3N4/4K3 w - - 0 1");
	cout << '\n';

	// Knight captures checking piece - legal
	cg.submitMove("E2", "E4");
	cout << '\n';

	cg.loadState("4k3/8/8/8/4q3/8/4R3/4K3 w - - 0 1");
	cout << '\n';

	// Rook pinned cannot move - illegal
	cg.submitMove("E2", "A2");
	cout << '\n';

	cg.loadState("8/8/8/8/8/6K1/4Q3/7k w - - 0 1");
	cout << '\n';

	// Only move creates stalemate - legal
	cg.submitMove("E2", "F2");
	cout << '\n';

	cg.loadState("4k3/8/8/8/4n3/3P4/8/4K3 w - - 0 1");
	cout << '\n';

	// Pawn takes diagonally - legal
	cg.submitMove("D3", "E4");
	cout << '\n';

	cg.loadState("4k3/8/8/8/4b3/8/4R3/4K3 w - - 0 1");
	cout << '\n';

	// King in discovered check after piece moves - illegal
	cg.submitMove("E2", "E4");
	cout << '\n';

	cg.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

	// Out of bounds input - return error
	cg.submitMove("J5", "B9");
	cout << '\n';

	// Invalid input format - return error
	cg.submitMove("12", "^%");
	cout << '\n';

	return 0;
}
