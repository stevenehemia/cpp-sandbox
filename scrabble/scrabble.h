enum ScoreModifier { NONE, DOUBLE_LETTER_SCORE, TRIPLE_LETTER_SCORE, DOUBLE_WORD_SCORE, TRIPLE_WORD_SCORE };

/* insert your function prototypes here */

int tile_score(char tile);
bool can_form_word_from_tiles(const char *word, const char *tiles, char played_tiles[]);
int compute_score(const char *played_tiles, ScoreModifier sm0[]);
int highest_scoring_word_from_tiles(const char *tiles, ScoreModifier sm1[], char word[]);
bool construct_word(const char *word, char *tiles, char played_tiles[]);
int letter_multiplier(ScoreModifier sm);
int word_multiplier(ScoreModifier sm);