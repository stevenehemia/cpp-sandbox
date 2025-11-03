#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "sudoku.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own
 * function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {

        cout << "Loading Sudoku board from file '" << filename << "'... ";

        ifstream in(filename);
        if (!in) {
                cout << "Failed!\n";
        }
        // assert(in);

        char buffer[512];

        int row = 0;
        in.getline(buffer,512);
        while (in && row < 9) {
                for (int n=0; n<9; n++) {
                        assert(buffer[n] == '.' || isdigit(buffer[n]));
                        board[row][n] = buffer[n];
                }
                row++;
                in.getline(buffer,512);
        }

        cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
        assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
        if (!(row % 3)) {
                cout << "  +===========+===========+===========+\n";
        } else {
                cout << "  +---+---+---+---+---+---+---+---+---+\n";
        }
}

/* internal helper function */
void print_row(const char* data, int row) {
        cout << (char) ('A' + row) << " ";
        for (int i=0; i<9; i++) {
                cout << ( (i % 3) ? ':' : '|' ) << " ";
                cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
        }
        cout << "|\n";
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
        cout << "    ";
        for (int r=0; r<9; r++) {
                cout << (char) ('1'+r) << "   ";
        }
        cout << '\n';
        for (int r=0; r<9; r++) {
                print_frame(r);
                print_row(board[r],r);
        }
        print_frame(9);
}

/* add your functions here */

/* function to check if the board is completely filled */
bool is_complete(const char board[][9]) {
        for (int r=0; r<9; r++) {
                for (int c=0; c<9; c++) {
                        if (board[r][c] < '0' || board[r][c] > '9') {
                                return false;
                        }
                }
        }
        return true;
}

/* function to fill a specified position with a specified digit on the board */
bool make_move(const char position[], const char digit, char board[][9]) {
        /* convert position from char type to array index */
        int row = index(position[0]);
        int col = index(position[1]);

        if(is_valid(row, col, digit, board)) {
                board[row][col] = digit;
                return true;
        }
        return false;
}

/* function to convert a character A to I or 1 to 9 to a respective integer */
int index(char ch) {
        if(ch >= '1' && ch <= '9') {                    /* for '1' to '9' (column) */
                return (static_cast<int>(ch) - 49);
        } else if (ch >= 'A' && ch <= 'I') {            /* for 'A' to 'I' (row) */
                return (static_cast<int>(ch) - 65);
        } else {                                        /* default case */
                return ch;
        }
}

/* function to check if putting a digit in the position is allowed */
bool is_valid(int row, int col, const char digit, const char board[][9]) {

        /* check if the position is out of bound */
        if(( row<0 || row>9 ) || ( col<0 || col>9 )) {
                return false;
        }

        /* check if there's duplicate in the row/column */
        for(int i=0; i<9; i++) {
                if(board[row][i] == digit) {
                        return false;
                }
                if(board[i][col] == digit) {
                        return false;
                }
        }

        /* check if there's duplicate in the 3x3 subgrid */
        for(int r=0 ;r<9; r++) {
                if(r/3 != row/3) {                      /* row/3 addresses row subgrid */
                        continue;
                }
                for(int c=0; c<9; c++) {
                        if(c/3 != col/3) {              /* col/3 addresses column subgrid */
                                continue;
                        }
                        if(board[r][c] == digit) {
                                return false;
                        }
                }
        }

        return true;
}

/* function to save the current board state to an external file */
bool save_board(const char* filename, char board[][9]) {
        cout << "Saving Sudoku board to " << filename << endl;

        ofstream out(filename);

        if(!out) {
                out.close();
                return false;
        } else {
                for(int r=0; r<9; r++) {
                        for(int c=0; c<9; c++) {
                                out.put(board[r][c]);
                        }
                        out.put('\n');
                }
        }
        out.close();
        return true;
}

/* function to solve a sudoku board */
bool solve_board(char board[9][9]) {
        static int recursions;
        recursions++;
        char position[2];
        bool is_empty = true;

        /* determine the location of an empty cell in the board */
        for(int r=0; r<9; r++) {
                for(int c=0; c<9; c++) {
                        if(board[r][c] == '.') {
                                position[0] = static_cast<char>('A'+r);
                                position[1] = static_cast<char>('1'+c);
                                is_empty = false;
                                break;
                        }
                }
                if(!is_empty) {
                        break;
                }
        }

        /* when all cells are filled, solution found */
        if(is_complete(board)) {
                cout << "Recurions: " << recursions << endl;
                recursions = 0;
                return true;
        }

        /* try to fill the cell with a digit 1 to 9, backtrack if the digit is not allowed */
        for(int i=0; i<9; i++) {
                char digit = '1' + i;
                if(make_move(position, digit, board)) { /* place the digit if possible */
                        if(solve_board(board)) {        /* continue to the next empty cell*/
                                return true;
                        }
                        /* when placing 1 to 9 not possible, set the current position to . and go back */
                        board[index(position[0])][index(position[1])] = '.';
                }
        }

        return false;
}