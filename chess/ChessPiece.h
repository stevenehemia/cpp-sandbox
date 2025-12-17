/**
 * @file    ChessPiece.h
 * @brief   Declaration of the Piece class and its derived classes.
 * @details This file defines the abstract Piece class representing a chess
 *          piece, along with derived classes for each specific piece type
 *          (Pawn, Knight, Bishop, Rook, Queen, King). It includes methods
 *          for calculating possible moves and attacks for each piece.
 */


#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <ostream>
#include <string>
#include <vector>

using std::string;


/* Forward declaration of ChessGame class */
class ChessGame;


/**
 * @brief Piece colour enumeration
 * @details WHITE representing the white pieces,
 *          BLACK representing the black pieces
 */
enum class PieceColour { WHITE, BLACK };

/**
 * @brief << operator override to output PieceColour
 * @param os Output stream
 * @param colour PieceColour to output
 */
std::ostream& operator<<(std::ostream& os, PieceColour colour);


/**
 * @brief Piece type enumeration
 * @details each type represents its corresponding chess piece
 */
enum class PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

/**
 * @brief << operator override to output PieceType
 * @param os Output stream
 * @param type PieceType to output
 */
std::ostream& operator<<(std::ostream& os, PieceType type);


/**
 * @brief Directional offsets for piece movement calculations
 * @details Used for generating possible moves for pieces
 *          that move in specific patterns.
 *          L_SHAPE: Knight moves
 *          ADJACENT: King moves
 *          DIAGONALS: Bishop moves
 *          STRAIGHTS: Rook moves
 *          QUEENS: Queen moves (combination of DIAGONALS and STRAIGHTS)
 */
const int L_SHAPE[8][2] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    {1, -2}, {1, 2}, {2, -1}, {2, 1}
};
const int ADJACENT[8][2] = {
    {0, 1}, {1, 0}, {0, -1}, {-1, 0},
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};
const int DIAGONALS[4][2] = {
    {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
};
const int STRAIGHTS[4][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};


/* ============================== Piece class ============================== */

class Piece
{
    protected:
        /* Class attributes */
        PieceType type;
        PieceColour colour;
        string position;
        ChessGame *game;

        /**
         * @brief Helper function to check if a position is within the
         *        board bounds
         * @param pos The position to check (e.g., "E4")
         * @return True if position is within bounds, false otherwise
         */
        bool isInBound(string pos) const;

        /**
         * @brief Helper function to check if a position is blocked
         *        by any piece
         * @param pos The position to check (e.g., "E4")
         * @return True if position is blocked, false otherwise
         */
        bool isBlocked(string pos) const;

    public:
        /* Vectors to store possible moves and attacks */
        std::vector<string> moves;
        std::vector<string> attacks;

        /**
         * @brief Constructor for Piece object (abstract class)
         * @param c Colour of the piece (WHITE or BLACK)
         * @param p Position of the piece (e.g., "E4")
         * @param g Pointer to the ChessGame instance
         */
        Piece(PieceColour c, string p, ChessGame* g);

        /* Destructor for Piece object */
        virtual ~Piece() = default;

        /**
         * @brief Calculate all possible moves for the piece and store
         *        them in the moves and attacks vectors
         * @details This virtual method is defined for all derived Piece
         *          classes, except Pawn.
         * @return void
         */
        virtual void calculateMoves();

        /**
         * @brief Helper function to add moves based on fixed offsets
         * @param offsets 2D array of offsets to apply for move generation,
         *               e.g., L_SHAPE for Knight & ADJACENT for King
         * @return void
         */
        void addOffsetMoves(const int offsets[8][2]);

        /**
         * @brief Helper function to add sliding moves along given directions
         * @param directions 2D array of directions to slide along,
         *                   e.g., DIAGONALS for Bishop & STRAIGHTS for Rook
         * @return void
         */
        void addSlidingMoves(const int directions[4][2]);

        /* Getters and setters */
        char getSymbol() const;
        PieceType getType() const;
        PieceColour getColour() const;
        string getPosition() const;
        void setPosition(const string& pos);
};


class Pawn: public Piece
{ 
    public:
        /**
         * @brief Constructor for Pawn object
         * @param c Colour of the pawn (WHITE or BLACK)
         * @param p Position of the pawn (e.g., "E4")
         * @param g Pointer to the ChessGame instance
         */
        Pawn(PieceColour c, string p, ChessGame* g);

        /* Destructor for Pawn object */
        ~Pawn() override = default;

        /**
         * @brief Override calculateMoves() for Pawn-specific movement
         *        and attack patterns. This method populates the moves
         *        and attacks vectors for the Pawn.
         * @return void
         */
        void calculateMoves() override;
};


class Knight: public Piece
{
    public:
        /**
         * @brief Constructor for Knight object
         * @param c Colour of the knight (WHITE or BLACK)
         * @param p Position of the knight (e.g., "E4")
         * @param g Pointer to the ChessGame instance
         */
        Knight(PieceColour c, string p, ChessGame* g);

        /* Destructor for Knight object */
        ~Knight() override = default;
};


class Bishop: public Piece
{
    public:
        /**
         * @brief Constructor for Bishop object
         * @param c Colour of the bishop (WHITE or BLACK)
         * @param p Position of the bishop (e.g., "E4")
         * @param g Pointer to the ChessGame instance
         */
        Bishop(PieceColour c, string p, ChessGame* g);

        /* Destructor for Bishop object */
        ~Bishop() override = default;
};


class Rook: public Piece
{
    public:
        /**
         * @brief Constructor for Rook object
         * @param c Colour of the rook (WHITE or BLACK)
         * @param p Position of the rook (e.g., "E4")
         * @param g Pointer to the ChessGame instance
         */
        Rook(PieceColour c, string p, ChessGame* g);

        /* Destructor for Rook object */
        ~Rook() override = default;
};


class Queen: public Piece
{
    public:
        /**
         * @brief Constructor for Queen object
         * @param c Colour of the queen (WHITE or BLACK)
         * @param p Position of the queen (e.g., "E4")
         * @param g Pointer to the ChessGame instance
         */
        Queen(PieceColour c, string p, ChessGame* g);

        /* Destructor for Queen object */
        ~Queen() override = default;
};


class King: public Piece
{
    public:
        /**
         * @brief Constructor for King object
         * @param c Colour of the king (WHITE or BLACK)
         * @param p Position of the king (e.g., "E4")
         * @param g Pointer to the ChessGame instance
         */
        King(PieceColour c, string p, ChessGame* g);

        /* Destructor for King object */
        ~King() override = default;
};

#endif
