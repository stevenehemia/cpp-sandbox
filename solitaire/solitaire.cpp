#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "solitaire.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a board */
bool get_board_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  cout << "Loading puzzle board from '" << filename << "'...";
  
  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }

  cout << " done (height = " << height << ", width = " << width << ")." << endl;
    
  return m;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << static_cast<char>('A' + c);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your function definitions below */

bool are_identical(char **first, char **second, int height, int width) {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (first[i][j] != second[i][j]) {
                return false;
            }
        }
    }
    return true;
}


bool make_move(char **board, const char *move, int height, int width) {

    int src_row = move[1] - '0';
    int src_col = move[0] - 'A';

    int dx = 0;
    int dy = 0;
    switch (move[2]) {
        case 'N': dy = -1; break;
        case 'S': dy = 1; break;
        case 'E': dx = 1; break;
        case 'W': dx = -1; break;
        default: return false;
    }

    int hop_row = src_row + dy;
    int hop_col = src_col + dx;
    if (!is_inbound(hop_row, hop_col, height, width)) { return false; }
    if (board[hop_row][hop_col] != 'O') { return false; }

    int dst_row = src_row + 2 * dy;
    int dst_col = src_col + 2 * dx;
    if (!is_inbound(dst_row, dst_col, height, width)) { return false; }
    if (board[dst_row][dst_col] != '_') { return false;}

    board[src_row][src_col] = '_';
    board[hop_row][hop_col] = '_';
    board[dst_row][dst_col] = 'O';

    return true;
}


bool find_solution(char **begin, char** end, int height, int width, char solution[]) {

    if (are_identical(begin, end, height, width) || strlen(solution) >= 512 - 4) {
        solution[strlen(solution) - 1] = '\0';
        return true;
    }

    const char DIRECTIONS[4] = { 'N', 'S', 'E', 'W' };

    for (char i = '0'; i <= '6'; i++) {
        for (char j = 'A'; j <= 'G'; j++) {
            if (begin[i - '0'][j - 'A'] != 'O') { continue; }
            for (char d : DIRECTIONS) {
                char move[] = { j, i, d, ',', '\0' };
                if (!make_move(begin, move, height, width)) { continue; }
                int len = strlen(solution);
                strcat(solution, move);
                if (find_solution(begin, end, height, width, solution)) { return true; }
                solution[len] = '\0';
                undo_move(begin, move);
            }
        }
    }

    return false;
}

bool is_inbound(int row, int col, int height, int width) {
    if (row < 0 || row >= height) { return false; }
    if (col < 0 || col >= width) { return false; }
    return true;
}

void undo_move(char **board, const char *move) {
    int src_row = move[1] - '0';
    int src_col = move[0] - 'A';

    int dx = 0;
    int dy = 0;
    switch (move[2]) {
        case 'N': dy = -1; break;
        case 'S': dy = 1; break;
        case 'E': dx = 1; break;
        case 'W': dx = -1; break;
    }

    int hop_row = src_row + dy;
    int hop_col = src_col + dx;

    int dst_row = src_row + 2 * dy;
    int dst_col = src_col + 2 * dx;

    board[src_row][src_col] = 'O';
    board[hop_row][hop_col] = 'O';
    board[dst_row][dst_col] = '_';
}