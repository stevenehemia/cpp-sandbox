#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */

#include <map>

map<char, int> letter_score = { {'A', 1}, {'B', 3}, {'C', 3}, {'D', 2},
                                {'E', 1}, {'F', 4}, {'G', 2}, {'H', 4},
                                {'I', 1}, {'J', 8}, {'K', 5}, {'L', 1},
                                {'M', 3}, {'N', 1}, {'O', 1}, {'P', 3},
                                {'Q', 10}, {'R', 1}, {'S', 1}, {'T', 1},
                                {'U', 1}, {'V', 4}, {'W', 4}, {'X', 8},
                                {'Y', 4}, {'Z', 10}, {'?', 0} };


int tile_score(char tile) {
    for (auto& letter : letter_score) {
        if (letter.first == toupper(tile)) { return letter.second; }
    }
    return -1;
}


bool can_form_word_from_tiles(const char *word, const char *tiles, char played_tiles[]) {

    char tiles_copy[8];
    strcpy(tiles_copy, tiles);
    played_tiles[0] = '\0';

    return construct_word(word, tiles_copy, played_tiles);
}


int compute_score(const char *played_tiles, ScoreModifier sm0[]) {

    int count, score = 0;

    for (count = 0; played_tiles[count] != '\0'; count++) {
        score += tile_score(played_tiles[count]) * letter_multiplier(sm0[count]);
    }

    for (int i = 0 ; i < count; i++) {
        score *= word_multiplier(sm0[i]);
    }

    if (strlen(played_tiles) == 7) { score += 50; }

    return score;
}


int highest_scoring_word_from_tiles(const char *tiles, ScoreModifier sm1[], char word[]) {

    int max_score = -1;
    char dict[80];
    char played_tiles[8];

    ifstream in_stream("words.txt");
    
    in_stream.getline(dict, 80);
    while(!in_stream.eof()) {
        if (can_form_word_from_tiles(dict, tiles, played_tiles)) {
            int score = compute_score(played_tiles, sm1);
            if (score > max_score) {
                max_score = score;
                strcpy(word, played_tiles);
            }
        }
        in_stream.getline(dict, 80);
    }

    in_stream.close();

    return max_score;
}


bool construct_word(const char *word, char *tiles, char played_tiles[]) {

    if (*word == '\0') { return true; }

    for (int i = 0; tiles[i] != '\0'; i++) {
        if (tiles[i] == *word || tiles[i] == '?') {
            int len = strlen(played_tiles);
            played_tiles[len] = tiles[i];
            played_tiles[len + 1] = '\0';
            char temp = tiles[i];
            tiles[i] = '.';
            if (construct_word(word + 1, tiles, played_tiles)) {
                return true;
            }
            played_tiles[len] = '\0';
            tiles[i] = temp;
        }
    }

    return false;
}


int letter_multiplier(ScoreModifier sm) {

    switch(sm) {
        case DOUBLE_LETTER_SCORE: return 2;
        case TRIPLE_LETTER_SCORE: return 3;
        default: return 1;
    }
}


int word_multiplier(ScoreModifier sm) {

    switch(sm) {
        case DOUBLE_WORD_SCORE: return 2;
        case TRIPLE_WORD_SCORE: return 3;
        default: return 1;
    }
}