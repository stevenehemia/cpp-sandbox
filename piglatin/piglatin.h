#ifndef PIGLATIN_H
#define PIGLATIN_H

#include <fstream>
#include <iostream>

int findFirstVowel(const char *word);
bool isVowel(char ch);
void translateWord(const char *english, char piglatin[]);
int wordLength(const char *word);
void formatWord(char word[], bool isCapital);
void translateStream(std::ifstream &inputStream, std::ostream &outputStream);

#endif