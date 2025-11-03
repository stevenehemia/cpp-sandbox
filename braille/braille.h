#ifndef BRAILLE_H
#define BRAILLE_H

#include <string>

using namespace std;

int encode_character(char ch, char braille[]);

void encode(string plaintext, char braille[]);

void print_braille(string plaintext, ostream& output);

#endif