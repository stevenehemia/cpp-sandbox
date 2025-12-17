#include "soundex.h"

char code_map[7][10] =
{
    {'0', 'a', 'e', 'h', 'i', 'o', 'u', 'w', 'y', '\0'},
    {'1', 'b', 'f', 'p', 'v', '\0'},
    {'2', 'c', 'g', 'j', 'k', 'q', 's', 'x', 'z', '\0'},
    {'3', 'd', 't', '\0'},
    {'4', 'l', '\0'},
    {'5', 'm', 'n', '\0'},
    {'6', 'r', '\0'}
};

void encode(const char *surname, char soundex[5])
{
    *soundex = *surname;

    char code = '0';
    char prev_code = '0';
    int cursor = 1;
    int count = 1;

    for (int i = 1; i < 4; i++) {
        while (surname[cursor] != '\0') {
            code = map_letter(surname[cursor]);
            if (code == '0' || code == prev_code) {
                prev_code = code;
                cursor++;
                continue;
            }
            soundex[i] = code;
            prev_code = code;
            cursor++;
            count++;
            break;
        }
    }

    for ( ; count < 4; count++) {
        soundex[count] = '0';
    }

    soundex[4] = '\0';
}

bool compare(const char one[], const char two[])
{
    // base case
    if (one[0] == '\0') { return true; }

    // mismatch
    if (one[0] != two[0]) { return false; }
    
    // recursive step
    return compare(one + 1, two + 1);
}

int count(const char *surname, const char *sentence)
{
    char surname_soundex[5];
    char word_soundex[5];

    encode(surname, surname_soundex);

    int cursor = 0;
    int occurence = 0;
    char word[20];
    int i = 0;

    while(sentence[cursor] != '\0') {

        if (sentence[cursor] == '.') { break; }

        char next_three[] = {sentence[cursor], sentence[cursor+1], sentence[cursor+2]};

        if (sentence[cursor] == ',' || compare("and", next_three)) {
            word[i] = '\0';
            encode(word, word_soundex);
            if (compare(surname_soundex, word_soundex)) { occurence++; }
            i = 0;
            cursor += 2;
            if (compare("and", next_three)) { cursor += 2; }
            continue;
        }

        word[i] = sentence[cursor];
        i++;
        cursor++;
    }

    return occurence;
}

char map_letter(const char ch)
{
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 10; j++) {
            if (ch == code_map[i][j]) {
                return code_map[i][0];
            }
        }
    }

    return '0';
}
