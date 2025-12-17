#ifndef SOUNDEX_H
#define SOUNDEX_H


void encode(const char *surname, char soundex[5]);
bool compare(const char one[], const char two[]);
int count(const char *surname, const char *sentence);
char map_letter(const char ch);


#endif