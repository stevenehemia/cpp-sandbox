#ifndef SUDOKU_H
#define SUDOKU_H

/**
 * Load a Sudoku board from a file into a 9x9 char array.
 * The file is expected to consist only of the characters
 * '1'-'9' and '.'.
 *
 * This function will throw an assertion error for malformed
 * files. Messages will be printed to the console to confirm
 * success and failure.
 *
 * @param filename The path of the file to be loaded, containing
 * a Sudoku board layout.
 * @param A pre-allocated char array to store the board data.
 */
void load_board(const char* filename, char board[9][9]);

/**
 * Print a formatted Sudoku board to the console.
 *
 * @param board Sudoku board data to be printed.
 */
void display_board(const char board[9][9]);

/**
 * Check whether a sudoku board is completely filled with digits
 *
 * @param sudoku board data to be checked
 * @return true if all 9x9 space are filled with digit, false otherwise
 * @notes each digit is not required to be logically valid
 */
bool is_complete(const char board[][9]);

/**
 * Place a digit onto a sudoku board at given position
 *
 * @param two chars string, i.e. "I8", specifying the position where the digit want to be placed
 * @param the digit that want to be placed
 * @param a sudoku board to be filled
 * @return true and update the board if the position and digit are valid
 * @return false and without updating the board if the position and digit are invalid
 * @notes each digit is not required to be logically valid
 */
bool make_move(const char position[], const char digit, char board[][9]);

/**
 * Convert a sudoku index (char type of 'A' to 'I' and '1' to '9' to board array index (integer type 1 to 9)
 *
 * @param a character A to I or a character 1 to 9
 * @return an integer 0 to 8 as mapped in the notes below
 * @notes A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8
 * @notes '1'=0, '2'=1, '3'=2, '4'=3, '5'=4, '6'=5, '7'=6, '8'=7, '9'=8
 */
int index(char ch);

/**
 * Save the current Sudoku board to a specified file
 *
 * @param a filename and a sudoku board
 * @return true if the saving successful and false otherwise
 */
bool save_board(const char* filename, char board[][9]);

/**
 * Save the current Sudoku board to a specified file
 *
 * @param a filename and a sudoku board
 * @return true if the saving successful and false otherwise
 */
bool is_valid(int row, int col, const char digit, const  char board[][9]);

/**
 * Solve the current sudoku board
 *
 * @param a sudoku board
 * @return true if the board is solvable, false otherwise
 * @notes the board will be filled with the correct digits
 */
bool solve_board(char board[9][9]);

#endif