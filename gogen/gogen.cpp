#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which removes unprintable characters like carriage returns and newlines from strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
  char **board = allocate_2D_array(5, 6);
  ifstream input(filename);
  assert(input);
  char buffer[512];
  int lines = 0;
  input.getline(buffer, 512);
  while (input && lines < HEIGHT) {
    filter(buffer);
    if (strlen(buffer) != WIDTH)
      cout << "WARNING bad input = [" << buffer << "]" << endl;
    assert(strlen(buffer) == WIDTH);
    strcpy(board[lines], buffer);
    input.getline(buffer, 512);
    lines++;
  }
  input.close();
  return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
  ofstream out(filename); 
  if (!out)
    return false;
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      out << board[r][c];
    }
    out << endl;
  }
  bool result = out.good();
  out.close();
  return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
  char word[512];
  int count = 0;
  ifstream in(filename);
  while (in >> word)
    count++;
  in.close();
  return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
  int count = count_words(filename);
  ifstream in(filename);
  assert(in);
  int n=0;
  char **buffer = new char *[count+1]; // +1 because we NULL terminate 
  char word[512];
  for (; (in >> word) && n<count; n++) {
    buffer[n] = new char[strlen(word) + 1];
    strcpy(buffer[n], word);
  }
  buffer[n] = NULL;
  in.close();
  return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      cout << "[" << board[r][c] << "]";
      if (c < WIDTH-1)
	cout << "--";
    }
    cout <<endl;
    if (r < HEIGHT-1) {
      cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
      cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
    }
  }
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
  for (int n=0; words[n]; n++) 
    cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) {
  deallocate_2D_array(board, HEIGHT);
}

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
  int count = 0;
  for (; words[count]; count++);
  deallocate_2D_array(words, count);
}

/* add your own function definitions here */

bool get_position(char **board, char ch, int &row, int &column) {

  for (int r = 0; r < HEIGHT; r++) {
    for (int c = 0; c < WIDTH; c++) {
      if (board[r][c] == ch) {
        row = r; column = c;
        return true;
      }
    }
  }

  return false;
}


bool valid_solution(char **board, char **words) {

  for (int i = 0; i < 7; i++) {
    if (!find_word(board, words[i])) {
      return false;
    }
  }

  return true;
}


void update(char **board, char ch, Mask &x) {

  int r, c;

  /* searching for ch in the board */
  if (get_position(board, ch, r, c)) {
    /* ch found */
    x.set_all(false);
    x.set_element(r, c, true);
    return;
  }

  int count = 0, row, col;
  /* count cells with value=true in mask */
  for (r = 0; r < HEIGHT; r++) {
    for (c = 0; c < WIDTH; c++) {
      if (x[r][c] == true) {
        /* found cells with value=true,
           increment count */
        count++; row = r; col = c;
      }
    }
  }

  if (count == 1) {
    /* there's only one cell with value true,
       set the cell value in the board to ch */
    board[row][col] = ch;
    return;
  }

  /* ch is not in the board & multiple true value in its mask*/
  x.set_all(false);
  for (r = 0; r < HEIGHT; r++) {
    for (c = 0; c < WIDTH; c++) {
      if (board[r][c] == ch || board[r][c] == '.') { x[r][c] = true; }
    }
  }
}


void neighbourhood_intersect(Mask &one, Mask &two) {
  two.intersect_with(one.neighbourhood());
  one.intersect_with(two.neighbourhood());
}


bool solve_board(char **board, char **words) {
  
  /* produce masks of all characters */
  Mask mask_list[25];
  update_list(board, mask_list);

  return solve_helper(board, words, mask_list);
}


bool find_word(char **board, char *word) {

  int anchor_row, anchor_col;

  if (!get_position(board, *word, anchor_row, anchor_col)) {
    return false;
  }

  /* array to iterate through indicating adjacent squares */
  const int ADJACENTS[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
                              {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

  for (unsigned int i = 0; word[i + 1] != '\0'; i++) {
    /* iterate through the characters int the word,
       look for the next character */
    char next_ch = word[i + 1];
    bool found = false;
    for (auto& adj : ADJACENTS) {

      /* look for next_ch in adjacent squares */
      int target_row = anchor_row + adj[0];
      int target_col = anchor_col + adj[1];

      /* boundary checks */
      if (target_row < 0 || target_row >= HEIGHT) { continue; }
      if (target_col < 0 || target_col >= WIDTH) { continue; }

      if (board[target_row][target_col] == next_ch) {
        /* next_ch found in one of the adjacent squares,
           set the target square as the anchor square */
        anchor_row = target_row;
        anchor_col = target_col;
        found = true;
        break;
      }
    }
    if (!found) { return false; }
  }
  return true;

}

bool solve_helper(char **board, char **words, Mask mask_list[]) {

  char **temp_board = allocate_2D_array(HEIGHT, WIDTH);
  Mask *temp_mask = new Mask[25];
  duplicate_list(temp_mask, mask_list);

  while (true) {
    duplicate_list(temp_mask, mask_list);
    refine_mask_list(board, words, mask_list);
    if (compare_list(temp_mask, mask_list)) { break; }
  }

  if (valid_solution(board, words)) { return true; }

  char ch = smallest_mask(mask_list);

  for (int r = 0; r < HEIGHT; r++) {
    for (int c = 0; c < WIDTH; c++) {
      if (mask_list[ch-'A'][r][c] == true) {
        duplicate_board(temp_board, board);
        duplicate_list(temp_mask, mask_list);
        board[r][c] = ch;
        if (solve_helper(board, words, mask_list)) {
          delete_board(temp_board);
          delete [] temp_mask;
          return true;
        }
        duplicate_board(board, temp_board);
        duplicate_list(mask_list, temp_mask);
      }
    }
  }
  delete_board(temp_board);
  delete [] temp_mask;

  return false;
}

void update_list(char **board, Mask mask_list[]) {
  for (char ch = 'A'; ch < 'Z'; ch++) {
    int idx = static_cast<int>(ch - 'A');
    update(board, ch, mask_list[idx]);
  }
}

void duplicate_list(Mask dst_list[], Mask src_list[]) {
  for (char ch = 'A'; ch < 'Z'; ch++) {
    int idx = static_cast<int>(ch - 'A');
    for (int r = 0; r < HEIGHT; r++) {
      for (int c = 0; c < WIDTH; c++) {
        dst_list[idx][r][c] = src_list[idx][r][c];
      }
    }
  } 
}

void duplicate_board(char **dst_board, char **src_board) {
  for (int r = 0; r < HEIGHT; r++) {
    for (int c = 0; c < WIDTH; c++) {
      dst_board[r][c] = src_board[r][c];
    }
  }
}

/* returns true if two lists are identical, false otherwise */
bool compare_list(Mask list_a[], Mask list_b[]) {
  for (char ch = 'A'; ch < 'Z'; ch++) {
    int idx = static_cast<int>(ch - 'A');
    Mask mask_a = list_a[idx], mask_b = list_b[idx];
    for (int r = 0; r < HEIGHT; r++) {
      for (int c = 0; c < WIDTH; c++) {
        if (mask_a[r][c] != mask_b[r][c]) { return false; }
      }
    }
  }
  return true;
}

void refine_mask_list(char **board, char **words, Mask mask_list[]) {
  for (char ch = 'A'; ch < 'Z'; ch++) {
    for (int i = 0; i < 7; i++) {
      for (unsigned int j = 0; j < strlen(words[i]); j++) {
        char current_char = words[i][j];
        if (current_char != ch) { continue; }
        int current_idx = static_cast<int>(current_char - 'A');
        if (j != 0) {
          char prev_char = words[i][j - 1];
          int prev_idx = static_cast<int>(prev_char - 'A');
          neighbourhood_intersect(mask_list[prev_idx], mask_list[current_idx]);
        }
        if (words[i][j + 1] != '\0') {
          char next_char = words[i][j + 1];
          int next_idx = static_cast<int>(next_char - 'A');
          neighbourhood_intersect(mask_list[current_idx], mask_list[next_idx]);
        }
      }
    }
    update(board, ch, mask_list[static_cast<int>(ch - 'A')]);
  }
}

char smallest_mask(Mask mask_list[]) {
  char smallest_char = 'A';
  int smallest_count = HEIGHT * WIDTH;
  for (char ch = 'A'; ch < 'Z'; ch++) {
    int count = 0;
    for (int r = 0; r < HEIGHT; r++) {
      for (int c = 0; c < WIDTH; c++) {
        if (mask_list[ch - 'A'][r][c] == true) {
          count++;
        }
      }
    }
    if (count < smallest_count) {
      smallest_count = count;
      smallest_char = ch;
    }
  }
  return smallest_char;
}