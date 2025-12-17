#include <iostream>
#include "correct.h"

using namespace std;


/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(char ch, char *binary) {
  for (int n = 128; n; n >>= 1) 
    *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}

/* converts a binary string representation into a character */
char binary_to_ascii(char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}

/* now add your own functions here */
void text_to_binary(const char *str, char *binary) {
  /* recursive version */
  if (*str == '\0') {
    *binary = '\0';
    return;
  }
  ascii_to_binary(*str, binary);
  text_to_binary(str + 1, binary + 8);


  /* iterative version
  int pos = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    char temp[8];
    ascii_to_binary(str[i], temp);
    for (int j = 0; j < 8; j++) {
        binary[pos] = temp[j];
        pos++;
    }
  }
  binary[pos] = '\0';
  */
}

void binary_to_text(const char *binary, char *str) {
  /* recursive version */
  if (*binary == '\0') {
    *str = '\0';
    return;
  }
  char temp[8];
  for (int i = 0; i < 8; i++) {
    temp[i] = binary[i];
  }
  *str = binary_to_ascii(temp);
  binary_to_text(binary + 8, str + 1);

  /* iterative version
  int pos = 0;
  int iter = 0;
  char temp[8];
  for (int i = 0; binary[i] != '\0'; i++) {
    temp[iter] = binary[i];
    iter++;
    if ((( i + 1 ) % 8) == 0) {
      str[pos] = binary_to_ascii(temp);
      pos++;
      iter = 0;
    }
  }
  */
}

void add_error_correction(const char *data, char *corrected) {
  char temp[8];
  int bit_index = 0;
  int pos = 0;
  for (int i = 0; data[i] != '\0'; i++) {
    temp[bit_index] = data[i];
    bit_index++;
    if ((( i + 1 ) % 4) == 0) {
      corrected[pos + 0] = parity(temp[0], temp[1], temp[3]);
      corrected[pos + 1] = parity(temp[0], temp[2], temp[3]);
      corrected[pos + 2] = temp[0];
      corrected[pos + 3] = parity(temp[1], temp[2], temp[3]);
      corrected[pos + 4] = temp[1];
      corrected[pos + 5] = temp[2];
      corrected[pos + 6] = temp[3];
      pos += 7;
      bit_index = 0;
    }
  }
  corrected[pos] = '\0';
}

int decode(const char *received, char *decoded) {
  int error = 0;
  char temp[8];
  int bit_index = 0;
  int pos = 0;
  for (int i = 0; received[i] != '\0'; i++) {
    temp[bit_index] = received[i];
    bit_index++;
    if ((( i + 1 ) % 7) == 0) {
      int p1 = static_cast<int>(parity(temp[3], temp[4], temp[5], temp[6]) - '0');
      int p2 = static_cast<int>(parity(temp[1], temp[2], temp[5], temp[6]) - '0'); 
      int p3 = static_cast<int>(parity(temp[0], temp[2], temp[4], temp[6]) - '0');
      if ((p1 + p2 + p3) > 0) {
          error++;
          int flip_pos = p1*4 + p2*2 + p3 - 1;
          temp[flip_pos] = temp[flip_pos] == '1' ? '0' : '1';
      }
      decoded[pos + 0] = temp[2];
      decoded[pos + 1] = temp[4];
      decoded[pos + 2] = temp[5];
      decoded[pos + 3] = temp[6];
      pos += 4;
      bit_index = 0;
    }
  }
  decoded[pos] = '\0';
  return error;
}

char parity(int a, int b, int c) {
  if (((a + b + c) % 2) == 0) { return '0'; }
  return '1';
}

char parity(int a, int b, int c, int d) {
  if (((a + b + c + d) % 2) == 0) { return '0'; }
  return '1';
}