#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <string>
#include <cctype>
#include <unistd.h>
#include <set>
#include <cstdint>

using namespace std;

// to control aspects of testing and solution animation
// see #define's for SIMPLE_TEST and ANIMATE_SOLUTION in sokoban.h

#include "sokoban.h"

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

/* internal helper function which gets the dimensions of a level */
bool get_level_dimensions(const char *filename, int &height, int &width) {
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

  //  cout << "get_level_dimensions(...): height = " << height << " width = " << width << endl;
  
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

/* pre-supplied function to load a level from a file*/
char **load_level(const char *filename, int &height, int &width) {

  cout << "Loading level from '" << filename << "'...";
  bool success = get_level_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **level = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    filter(line);
    while ( (int) strlen(line) < width) {
      char sp[2] = " ";
      strcat(line, sp);
    }
    strcpy(level[r], line);

  }
  cout << " done (height = " << height << ", width = " << width << ")." << endl;  
  return level;
}

/* pre-supplied function to print a level */
void print_level(char **level, int height, int width) {
  if (width > 10) {
    cout << setw(4) << " " << " ";
    for (int c=0; c<width; c++)
      if (c && (c % 10) == 0) 
	cout << c/10;
      else
	cout << " ";
    cout << endl;
  }

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << level[r][c];
    cout << endl;
  }
}

/* pre-supplied function to map a level onto an unsigned 64-bit integer */
uint64_t level_hash(char **level, int height, int width) {
  uint64_t hash = 0, sum = 0;

  int s1 = 0, s2=16;

  for (int r=0; r<height; r++)
    for (int c=0; c<width; c++) {
      sum += level[r][c];
      hash ^= (level[r][c] << s1);
      hash ^= (sum << s2);
      s1 += 3;
      s2 += 5;
      s1 %= 57;  
      s2 %= 57;  
    }
  return hash;
}

#ifdef ANIMATE_SOLUTION
void animate_solution(char **level, int height, int width, const char *solution) {
  print_level(level, height, width);
  bool is_push = false;
  for (unsigned int n=0; n<strlen(solution); n++) {
    cout << "trying " << solution[n] << endl;
    assert(make_move(level, height, width, solution[n], is_push));
    for (int rows=height; rows<SCREEN_HEIGHT; rows++)
      cout << endl;    
    print_level(level, height, width);
    usleep(100000);
  }
}
#endif

/* add your own function definitions here */

#include <set>

const char MOVES[] = { 'u', 'r', 'd', 'l' };

int goal_squares_without_boxes(char **level, int height, int width) {

    int count = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (level[i][j] == '.' || level[i][j] == '+') { count++; }
        }
    }
    return count;
}


bool find_player(char **level, int height, int width, int &row, int &column) {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (level[i][j] == '@' || level[i][j] == '+') {
                row = i; column = j;
                return true;
            }
        }
    }
    return false;
}


bool make_move(char **level, int height, int width, char dir, bool &is_push) {

    int src_col, src_row;
    if (!find_player(level, height, width, src_row, src_col)) { return false; }

    int dx = 0, dy = 0;
    set_direction(dir, dx, dy);
    int dst_col = src_col + dx;
    int dst_row = src_row + dy;

    if(dst_col < 0 || dst_col > width) { return false; }
    if(dst_row < 0 || dst_row > height) { return false; }
    if (level[dst_row][dst_col] == '#') { return false; }

    if (level[dst_row][dst_col] == '.' || level[dst_row][dst_col] == ' ') {
        level[dst_row][dst_col] = level[dst_row][dst_col] == '.' ? '+': '@'; 
        level[src_row][src_col] = level[src_row][src_col] == '+' ? '.': ' ';
        return true;
    }

    if (level[dst_row][dst_col] == '$' || level[dst_row][dst_col] == '*') {
        int next_col = dst_col + dx;
        int next_row = dst_row + dy;
        if (level[next_row][next_col] == '.' || level[next_row][next_col] == ' ') {
            level[next_row][next_col] = level[next_row][next_col] == '.' ? '*': '$';
            level[dst_row][dst_col] = level[dst_row][dst_col] == '*' ? '+': '@';
            level[src_row][src_col] = level[src_row][src_col] == '+' ? '.': ' ';
            is_push = true;
            return true;
        }
        return false;
    }

    return true;
}


bool solve_level(char **level, int height, int width, char *solution) {

    set<uint64_t> visited;

    return search_solution(level, height, width, solution, visited);
}


bool search_solution(char **level, int height, int width, char *solution, set<uint64_t> &visited) {

    if (goal_squares_without_boxes(level, height, width) == 0 ||
        strlen(solution) > MAX_SOLUTION_LENGTH) {
            return true;
    }

    for (char move : MOVES) {

        char **backup = allocate_2D_array(height, width);
        copy_level(backup, level, height, width);
        bool is_push = false;

        if(make_move(level, height, width, move, is_push)) {

            uint64_t current_hash = level_hash(level, height, width);

            if (visited.count(current_hash) || is_cornered(level, height, width)) {
                copy_level(level, backup, height, width);   
                continue;
            }

            visited.insert(current_hash);
            if (is_push) { move = toupper(move); }
            int len = strlen(solution);
            solution[len] = move;
            solution[len + 1] = '\0';

            if(search_solution(level, height, width, solution, visited)) { return true; }

            solution[len] = '\0';
            copy_level(level, backup, height, width);
        }
        
        deallocate_2D_array(backup, height);
    }

    return false;
}


void copy_level(char **dst, char **src, int height, int width) {
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            dst[r][c] = src[r][c];
        }
    }
}


void set_direction(char dir, int &dx, int &dy) {
    switch (dir) {
        case 'l': dx = -1; dy = 0; break;
        case 'r': dx = 1; dy = 0; break;
        case 'd': dx = 0; dy = 1; break;
        case 'u': dx = 0; dy = -1; break;
    }
}

bool is_cornered(char **level, int height, int width) {
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            if (level[r][c] == '$') {
                for (int i = 0; i < 4; i++) {
                    int dx = 0, dy = 0;
                    int idx = i;
                    set_direction(MOVES[idx], dx, dy);
                    int pos1_row = r + dy;
                    int pos1_col = c + dx;
                    idx = i == 3 ? 0 : i + 1;
                    set_direction(MOVES[idx], dx, dy);
                    int pos2_row = r + dy;
                    int pos2_col = c + dx;
                    if (level[pos1_row][pos1_col] == '#' &&
                        level[pos2_row][pos2_col] == '#') {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}