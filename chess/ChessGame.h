/**
 * @file    ChessGame.h
 * @brief   Declaration of the ChessGame class for managing chess game state.
 *          Provides methods for game initialization, move submission,
 *          validation, and game state updates.
 * @details This class encapsulates the overall state of a chess game,
 *          including the chessboard, active turn, castling rights, and move
 *          counters. It provides methods to load game state from FEN,
 *          submit and validate moves, check for check/checkmate/stalemate,
 *          and update the game state after each move.
 */


#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <iostream>
#include <string>
#include <cctype>
#include <memory>
#include "ChessBoard.h"

using std::string;


/**
 * @brief Move type enumeration
 * @details STANDARD for normal moves,
 *          CAPTURE for capturing moves,
 *          CASTLE for castling moves,
 *          ILLEGAL for illegal moves.
 */
enum class MoveType { STANDARD, CAPTURE, CASTLE, ILLEGAL };


/* ============================ ChessGame class ============================ */

class ChessGame
{
    private:
        /* Chess board representing the current state of the game */
        ChessBoard board;
        
        /* Initial game state in FEN notation for backup*/
        string initState;
        
        /* Piece placement in FEN notation */
        string placement;
        
        /* Active turn indicator */
        PieceColour activeTurn;

        /* Castling rights */
        bool whiteCastleKing;
        bool whiteCastleQueen;
        bool blackCastleKing;
        bool blackCastleQueen;
        
        /* En passant target square */
        string enPassant;
        
        /* Halfmove clock and fullmove count */
        int halfmoveClock;
        int fullmoveCount;

        /* Pointers to the kings for quick access */
        King* whiteKing;
        King* blackKing;

    public:
        /* Constructor for ChessGame */
        ChessGame();

        /**
         * @brief Loads the game state from a FEN string
         * @param FEN The FEN string representing the game state
         * @return void
         * @details This method parses the FEN string to set up the
         *          chessboard, active turn, castling rights, en passant
         *          target square, and move counters.
         */
        void loadState(string FEN);

        /**
         * @brief Populates the chessboard based on piece placement string
         * @param plcm The piece placement string in FEN notation
         * @return void
         * @details This method interprets the piece placement portion of
         *          the FEN string to set up the chessboard.
         */
        void populateBoard(string plcm);

        /**
         * @brief Creates and places a piece on the board
         * @param p Character representing the piece type and colour
         * @param pos Position to place the piece (e.g., "E4")
         * @return void
         * @details This method creates a Piece object based on its character
         *          representation in FEN notation and places it on the
         *          specified position on the board.
         */
        void createPiece(char p, string pos);

        /**
         * @brief Submits a move from one position to another
         * @param from The starting position (e.g., "E2")
         * @param to The target position (e.g., "E4")
         * @return void
         * @details This method handles move submission, including
         *          validation, printing move details to console,
         *          and updating the game state accordingly.
         */
        void submitMove(string from, string to);

        /**
         * @brief Helper function to validate input positions
         * @param from The starting position (e.g., "E2")
         * @param to The target position (e.g., "E4")
         * @return True if both positions are valid, false otherwise
         */
        bool isInputValid(string from, string to) const;

        /**
         * @brief Helper function to validate if a piece exists at the given
         *        position
         * @param pos The position to check (e.g., "E4")
         * @return True if a piece exists at the position, false otherwise
         */
        bool isPieceExist(string pos) const;

        /**
         * @brief Helper function to validate if it's the correct turn for
         *        the piece at the given position
         * @param pos The position to check (e.g., "E4")
         * @return True if it's the correct turn, false otherwise
         */
        bool isRightTurn(string pos) const;

        /**
         * @brief Evaluate if a move is pseudo-legal (ignores checks)
         * @param from The starting position (e.g., "E2")
         * @param to The target position (e.g., "E4")
         * @return MoveType indicating legality and type of move
         * @details This method checks if the move conforms to piece
         *          movement rules, without considering checks.
         */
        MoveType isPseudoLegal(string from, string to);

        /**
         * @brief Evaluate if a move is fully legal (considering checks)
         * @param from The starting position (e.g., "E2")
         * @param to The target position (e.g., "E4")
         * @return MoveType indicating legality and type of move
         * @details This method checks if the move is pseudo-legal and
         *          does not leave the moving side in check.
         */
        MoveType isLegal(string from, string to);

        /**
         * @brief Evaluate if a position is in check for a given colour
         * @param clr The colour to check for (WHITE or BLACK)
         * @return True if in check, false otherwise
         */
        bool isInCheck(PieceColour clr) const;

        /**
         * @brief Evaluate if the given colour is in checkmate
         * @param clr The colour to check for (WHITE or BLACK)
         * @return True if in checkmate, false otherwise
         */
        bool isCheckmate(PieceColour clr);

        /**
         * @brief Evaluate if the given colour is in stalemate
         * @param clr The colour to check for (WHITE or BLACK)
         * @return True if in stalemate, false otherwise
         */
        bool isStalemate(PieceColour clr);

        /**
         * @brief Evaluate if the given colour has any legal moves
         * @param clr The colour to check for (WHITE or BLACK)
         * @return True if there are legal moves, false otherwise
         * @details This method scans all pieces of the given colour and
         *          checks if at least one legal move exists.
         */
        bool hasLegalMoves(PieceColour clr);

        /**
         * @brief Check if a square is attacked by any piece of a given colour
         * @param pos The position to check (e.g., "E4")
         * @param byColour The colour of the attacking pieces (WHITE or BLACK)
         * @return True if the square under attack by at least one piece,
         *         false otherwise
         * @details This method scans the board for pieces of the specified
         *          colour and checks if any can attack the given position.
         */
        bool isSquareAttacked(const string pos, PieceColour byColour) const;

        /**
         * @brief Helper function to check if castling is legal
         * @param from The starting position of the king (e.g., "E1")
         * @param to The target position of the king (e.g., "G1" or "C1")
         * @return True if the king can castle to the specified position,
         *         false otherwise
         * @details This method checks all conditions for castling legality,
         *          i.e. still has castling right, the squares inbetween them
         *          are unoccupied, and the king does not pass through or end
         *          up in check.
         */
        bool canCastle(const string& from, const string& to) const;

        /**
         * @brief Method to update game state after a move
         * @param mt The type of move made
         * @param to The target position of the move (e.g., "E4")
         * @return void
         * @details This method updates castling rights, and move counters
         *          based on the move made.
         */
        void updateGameState(MoveType mt, string to);

        /**
         * @brief Method to remove a piece from the board
         * @param pos The position to remove the piece from (e.g., "E4")
         * @return A unique pointer to the removed Piece object
         */
        std::unique_ptr<Piece> removePiece(string pos);

        /**
         * @brief Displays the current state of the chessboard to console
         * @return void
         * @details This method prints the chessboard in a human-readable
         *          format to the console.
         */
        void display() const;

        // Getters and setters
        Piece* getPiece(string pos) const;
        void setPiece(string pos, std::unique_ptr<Piece> p);
};

#endif
