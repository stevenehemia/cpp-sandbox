/**
 * @file    ChessBoard.h
 * @brief   Declaration of the ChessBoard class for managing the chessboard
 *          state. Provides methods for piece placement, removal, and board
 *          display.
 * @details This class uses a 2D array of unique pointers to Piece objects
 *          to represent the chessboard. It includes methods to access and
 *          modify the pieces on the board, as well as to display the current
 *          state of the board.
 */


#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <string>
#include <memory>
#include "ChessPiece.h"

using std::string;


/* ========================= ChessBoard class ========================= */

class ChessBoard
{
    private:
        /* 2D array representing the chessboard grid */
        std::unique_ptr<Piece> grid[8][8];

    public:

        /**
         * @brief Helper object returned by ChessBoard::operator[].
         * @details Allows double indexing the board using chess notation
         *          (e.g., board['e']['4'] to access the piece at e4).
         *          The first indexing step stores the file (column) and
         *          the second indexing step resolves the rank (row).
         */
        struct RankAccessor
        {
            int file;
            ChessBoard* board;
            std::unique_ptr<Piece>& operator[](char rankChar);
            const std::unique_ptr<Piece>& operator[](char rankChar) const;
        };
        RankAccessor operator[](char fileChar);
        const RankAccessor operator[](char fileChar) const;

        /**
         * @brief Constructor for ChessBoard class.
         *        Initializes an empty chessboard.
         */
        ChessBoard();

        /**
         * @brief Removes and returns the piece at the specified position.
         * @param pos The position on the board (e.g., "E4").
         * @return A unique pointer to the removed Piece object.
         */
        std::unique_ptr<Piece> removePieceAt(string pos);

        /**
         * @brief Displays the current state of the chessboard.
         * @return void
         * @details Prints the chessboard in a human-readable format to
         *          the console.
         */
        void display() const;


        /* Getters and setters */
        Piece* getPieceAt(string pos) const;
        void setPieceAt(string pos, std::unique_ptr<Piece> piece);
};

#endif
