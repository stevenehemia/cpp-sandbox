#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "laser.h"

using namespace std;

/* pre-supplied helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **array = new char *[rows];
  assert(array);
  for (int r=0; r<rows; r++) {
    array[r] = new char[columns];
    assert(array[r]);
  }
  return array;
}

/* pre-supplied helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **array, int rows) {
  for (int r=0; r<rows; r++)
    delete [] array[r];
  delete [] array;
}

/* internal helper function which gets the dimensions of a board */
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

/* internal helper function which removes carriage returns and newlines from end of strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **board = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    filter(line);
    strcpy(board[r], line);
  }
  
  return board;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your own function definitions here */

const char OBJECTS[] = { '/', '\\', ' '};


bool find_laser(char **board, int height, int width, int &row) {

    for (int r = 0; r < height; r++) {
        if (board[r][0] == '>') {
            row = r;
            return true;
        }
    }
    return false;
}


char mirror_label(char **board, int height, int width, int row, int column) {

    const int ADJACENT[8][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0},
                                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (auto d : ADJACENT) {
      char check_sq = board[row + d[0]][column + d[1]];
      if (isalpha(check_sq)) { return check_sq; } 
    }
    return '\0';
}


bool shoot(char **board, int height, int width, char message[], int &last_row, int &last_col) {
    
    message[0] = '\0';

    int start_row, start_col = 1;
    if (!find_laser(board, height, width, start_row)) { return false; }

    int dx = 1, dy = 0;
    char current_dir = 'r';
    for (int r = start_row, c = start_col ; ; r += dy, c += dx) {

      if (board[r][c] == '@') {
        last_row = r; last_col = c;
        return true;
      }

      if (board[r][c] == '#' || board[r][c] == '?') {
        last_row = r; last_col = c;
        return false;
      }

      if (board[r][c] == '/' || board[r][c] == '\\') {
        message[strlen(message) + 1] = '\0';
        message[strlen(message)] = mirror_label(board, height, width, r, c);
        current_dir = change_direction(board[r][c], current_dir);
        set_direction(current_dir, dx, dy);
        continue;
      }

      if (board[r][c] == '-') {
        if (current_dir == 'u' || current_dir == 'd') {
          board[r][c] = '+';
          continue;
        }
      }

      if(board[r][c] == '|')  {
        if (current_dir == 'l' || current_dir == 'r') {
          board[r][c] = '+';
          continue;
        }
      }

      if (board[r][c] == ' ') {
        if (current_dir == 'u' || current_dir == 'd') { board[r][c] = '|'; }
        if (current_dir == 'l' || current_dir == 'r') { board[r][c] = '-'; }
      }
    }

    return false;
}


bool solve(char **board, int height, int width, const char *target) {

  int last_row, last_col;
  char message[512];

  if (shoot(board, height, width, message, last_row, last_col)) {
    if(!strcmp(message, target)) { return true; }
  }

  if (board[last_row][last_col] == '?') {
    for (char object : OBJECTS) {
      board[last_row][last_col] = object;
      if (solve(board, height, width, target)) { return true; }
    }
    remove_trace(board, height, width);
    board[last_row][last_col] = '?';
  }

  return false;
}


void set_direction(char dir, int &dx, int &dy) {
  switch (dir) {
    case 'u': dx = 0; dy = -1; break;
    case 'd': dx = 0; dy = 1; break;
    case 'l': dx = -1; dy = 0; break;
    case 'r': dx = 1; dy = 0; break;
    default: dx = 0; dy = 0;
  }
}


char change_direction(char mirror, char dir) {
    switch (dir) {
      case 'u': return (mirror == '/') ? 'r' : 'l';
      case 'd': return (mirror == '/') ? 'l' : 'r';
      case 'l': return (mirror == '/') ? 'd' : 'u';
      case 'r': return (mirror == '/') ? 'u' : 'd';
      default: return '\0';
    }
}


void remove_trace(char **board, int height, int width) {
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      if (board[r][c] == '-' || board[r][c] == '|' || board[r][c] == '+') {
        board[r][c] = ' ';
      }
    }
  }
}