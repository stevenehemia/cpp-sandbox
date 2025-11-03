#include <iostream>
#include <string>

using namespace std;

int encode_character(char ch, char braille[])
{
    int size = 0;
    string prefix = "", code, combined;

    if(isupper(ch)) {
        size = size + 6;
        prefix = ".....0";
    }

    if(isdigit(ch)) {
        size = size + 6;
        prefix = ".0.000";
    }

    size = size + 6;
    switch(tolower(ch)) {
        case 'a': code = "0....."; break;
        case 'b': code = "0.0..."; break;
        case 'c': code = "00...."; break;
        case 'd': code = "00.0.."; break;
        case 'e': code = "0..0.."; break;
        case 'f': code = "000..."; break;
        case 'g': code = "0000.."; break;
        case 'h': code = "0.00.."; break;
        case 'i': code = ".00..."; break;
        case 'j': code = ".000.."; break;
        case 'k': code = "0...0."; break;
        case 'l': code = "0.0.0."; break;
        case 'm': code = "00..0."; break;
        case 'n': code = "00.00."; break;
        case 'o': code = "0..00."; break;
        case 'p': code = "000.0."; break;
        case 'q': code = "00000."; break;
        case 'r': code = "0.000."; break;
        case 's': code = ".00.0."; break;
        case 't': code = ".0000."; break;
        case 'u': code = "0...00"; break;
        case 'v': code = "0.0.00"; break;
        case 'w': code = ".00..0"; break;
        case 'x': code = "00..00"; break;
        case 'y': code = "00.000"; break;
        case 'z': code = "0..000"; break;

        case '1': code = "0....."; break;
        case '2': code = "0.0..."; break;
        case '3': code = "00...."; break;
        case '4': code = "00.0.."; break;
        case '5': code = "0..0.."; break;
        case '6': code = "000..."; break;
        case '7': code = "0000.."; break;
        case '8': code = "0.00.."; break;
        case '9': code = ".00..."; break;
        case '0': code = ".000.."; break;

        case '.': code = "..00.0"; break;
        case ',': code = "..0..."; break;
        case ';': code = "..0.0."; break;
        case '-': code = "....00"; break;
        case '!': code = "..000."; break;
        case '?': code = "..0.00"; break;
        case '(': code = "..0000"; break;
        case ')': code = "..0000"; break;

        default: code = "......";
    }

        combined = prefix + code;

        for(int i = 0; i < combined.size(); i++) {
            braille[i] = combined[i];
        }
        braille[combined.size()] = '\0';

        return size;
}

void encode(string plaintext, char braille[])
{
    static int size = 0;
    int base = size;
    char encoding[13];

    if(plaintext.size() == 0) {
        size = 0;
        return;
    }
    
    size += encode_character(plaintext[0], encoding);
    for(int i = base; i < size; i++) {
        braille[i] = encoding[i - base];
    }

    encode(plaintext.erase(0,1), braille);
}

void print_braille(string plaintext, ostream& output)
{
    string row1, row2, row3, row4;
    char encoding[13];
    int size;

    for(int i = 0; i < plaintext.size(); i++) {
        size = encode_character(plaintext[i], encoding);
        if(size == 12) {
            row1 = row1 + encoding[0] + encoding[1] + " " + encoding[6] + encoding[7] + " ";
            row2 = row2 + encoding[2] + encoding[3] + " " + encoding[8] + encoding[9] + " ";
            row3 = row3 + encoding[4] + encoding[5] + " " + encoding[10] + encoding[11] + " ";
            row4 = row4 + "   " + plaintext[i] + "  ";
        } else {
            row1 = row1 + encoding[0] + encoding[1] + " ";
            row2 = row2 + encoding[2] + encoding[3] + " ";
            row3 = row3 + encoding[4] + encoding[5] + " ";
            row4 = row4 + plaintext[i] + "  ";
        }
    }

    output << row1 << endl;
    output << row2 << endl;
    output << row3 << endl;
    output << row4 << endl;
}