/*  Pre-supplied functions for loading and printing board and word list */

char **load_board(const char *filename);
bool save_board(char **board, const char *filename);
void print_board(char **board);

char **load_words(const char *filename);
void print_words(char **words);

void delete_board(char **board);
void delete_words(char **words);

/* -----------8<-------------8<------------8<--------------8<------------ */

/* add your function prototypes here */

bool get_position(char **board, char ch, int &row, int &column);
bool valid_solution(char **board, char **words);
void update(char **board, char ch, Mask &x);
void neighbourhood_intersect(Mask &one, Mask &two);
bool solve_board(char **board, char **words);
bool find_word(char **board, char *word);
bool solve_helper(char **board, char **words, Mask m[]);
void update_list(char **board, Mask mask_list[]);
void duplicate_board(char **dst_board, char **src_board);
void duplicate_list(Mask dst_list[], Mask src_list[]);
bool compare_list(Mask list_a[], Mask list_b[]);
void refine_mask_list(char **board, char **words, Mask mask_list[]);
char smallest_mask(Mask mask_list[]);