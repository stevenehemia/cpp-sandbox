#include <iostream>
#include <fstream>
#include <cctype>
#include "piglatin.h"


int findFirstVowel(const char *word)
{
    bool found = false;
    int firstVowel = -1;
    int i = 0;

    while (!found && word[i] != '\0') {
        if (isVowel(word[i])) {
            if (tolower(word[i]) == 'y') {
                if (i == 0 || word[i + 1] == '\0') {
                    i++;
                    continue;
                }
            }
            firstVowel = i;
            found = true;
            break;
        }
        i++;
    }

    return firstVowel;
}


bool isVowel(char ch)
{
    char vowelList[] = {'a', 'e', 'i', 'o', 'u', 'y'};
    
    for (int i = 0; i < 6; i++) {
        if (tolower(ch) == vowelList[i]) {
            return true;
        }
    }

    return false;
}


void translateWord(const char *english, char piglatin[])
{
    int len = wordLength(english);

    if (len == 0) { return; }

    if (isdigit(english[0])) {
        for(int i = 0; i < len; i++) {
            piglatin[i] = english[i];
        }
        piglatin[len] = '\0';
        return;
    }

    int firstVowel = findFirstVowel(english);

    for (int i = firstVowel; i < len; i++) {
        piglatin[i - firstVowel] = english[i];
    }
    for (int j = 0; j < firstVowel; j++) {
        piglatin[len - firstVowel + j] = english[j];
    }

    int k = 0;
    if (firstVowel == 0) {
        piglatin[len + k] = 'w';
        k++;
    }
    piglatin[len + k] = 'a';
    piglatin[len + k + 1] = 'y';
    piglatin[len + k + 2] = '\0';

    if (isupper(english[0])) {formatWord(piglatin, true); }
    else { formatWord(piglatin, false); }
}


int wordLength(const char *word)
{
    int count = 0;
    while (word[count] != '\0') { count++; }
    return count;
}


void formatWord(char word[], bool isCapital)
{
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }

    if (isCapital) { word[0] = toupper(word[0]); }
}


void translateStream(std::ifstream &inputStream, std::ostream &outputStream)
{
    char ch;

    if (!inputStream.get(ch)) {
        return;
    }

    char word[10];
    char translated[13];

    int i = 0;
    while (isalnum(ch)) {
        word[i] = ch;
        i++;
        if (!inputStream.get(ch)) {
            break;
        }
    }
    word[i] = '\0';

    translateWord(word, translated);
    outputStream << translated;

    outputStream << ch;

    translateStream(inputStream, outputStream);

    /* iterative version
    char ch;
    char word[10];
    char translated[13];

    inputStream.get(ch);
    int i = 0;
    while (!inputStream.eof()) {
        if (isalnum(ch)) {
            word[i] = ch;
            i++;
            inputStream.get(ch);
            continue;
        }
        word[i] = '\0';
        i = 0;
        translateWord(word, translated);
        outputStream << translated;
        word[0] = '\0';
        translated[0] = '\0';
        outputStream << ch;
        inputStream.get(ch);
    }
    */
}