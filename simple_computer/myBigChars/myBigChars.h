#ifndef MY_BIG_CHARS_H
#define MY_BIG_CHARS_H

#include <inttypes.h>
#include <myTerm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct BigChars
{
  unsigned int zero[2];
  unsigned int one[2];
  unsigned int two[2];
  unsigned int three[2];
  unsigned int four[2];
  unsigned int five[2];
  unsigned int six[2];
  unsigned int seven[2];
  unsigned int eight[2];
  unsigned int nine[2];
  unsigned int A[2];
  unsigned int B[2];
  unsigned int C[2];
  unsigned int D[2];
  unsigned int E[2];
  unsigned int F[2];
  unsigned int plus[2];
  unsigned int minus[2];
};

extern struct BigChars bch;

/*
0 : 00111100 01000010 10000001 10000001 10000001 10000001 01000010 00111100 ==
arr[0] = 0x3c428181; arr[1] = 0x8181423c
1 : 00111000 01111000 11011000
00011000 00011000 00011000 00011000 01111110 == arr[0] = 0x3878d818; arr[1] =
0x1818187e
2 : 01111110 11000011 00000001 00000110 00011000 01100000 10000000
11111111 == arr[0] = 0x7ec30106; arr[1] = 0x186080ff
3 : 11111111 00000010
00000100 00001111 00000001 00000001 10000001 01111110 == arr[0] = 0xff02040f;
arr[1] = 0x0101817e
4 : 00001110 00010010 00100010 01000010 11111111 00000010
00000010 00000010 == arr[0] = 0x0e122242; arr[1] = 0xff020202
5 : 11111111
10000000 10000000 11111111 00000001 00000001 00000001 11111111 == arr[0] =
0xff8080ff; arr[1] = 0x010101ff
6 : 00011000 00110000 01100000 11000000
11111111 10000001 10000001 11111111 == arr[0] = 0x183060c0; arr[1] = 0xff8181ff
7 : 11111111 00000011 00000110 00001100 00011000 00110000 01100000 11000000 ==
arr[0] = 0xff03060c; arr[1] = 0x183060c0
8 : 11111111 10000001 10000001
10000001 11111111 10000001 10000001 11111111 == arr[0] = 0xff818181; arr[1] =
0xff8181ff
9 : 11111111 10000001 10000001 11111111 00000011 00000110 00001100
00011000 == arr[0] = 0xff8181ff; arr[1] = 0x03060c18
A : 00111100 01100110
11000011 10000001 11111111 10000001 10000001 10000001 == arr[0] = 0x3c66c381;
arr[1] = 0xff818181
B : 11111110 11000001 11000001 11000001 11111110 11000001
11000001 11111110 == arr[0] = 0xfec1c1c1; arr[1] = 0xfec1c1fe
C : 00111111
11000000 11000000 11000000 11000000 11000000 11000000 00111111 == arr[0] =
0x3fc0c0c0; arr[1] = 0xc0c0c03f
D : 11111110 11000001 11000001 11000001
11000001 11000001 11000001 11111110 == arr[0] = 0xfec1c1c1; arr[1] = 0xc1c1c1fe
E : 11111111 11000000 11000000 11000000 11111111 11000000 11000000 11111111 ==
arr[0] = 0xffc0c0c0; arr[1] = 0xffc0c0ff
F : 11111111 11000000 11000000 11000000 11111100 11000000 11000000 11000000 ==
arr[0] = 0xffc0c0c0; arr[1] = 0xfcc0c0c0
+ : 00011000 00011000 00011000 11111111 11111111 00011000 00011000 00011000 ==
arr[0] = 0x181818ff; arr[1] = 0xff181818
- : 00000000 00000000 00000000 11111111 11111111 00000000 00000000 00000000 ==
arr[0] = 0x000000ff; arr[1] = 0xff000000
*/

void bc_init_big_chars ();
unsigned int *get_bigchar (uint8_t buf);
int bc_printA (char *str);
int bc_box (int x1, int y1, int x2, int y2);
int bc_printbigchar (unsigned int arr[2], int x, int y, colors back_color,
                     colors fore_color);
int bc_setbigcharpos (unsigned int *big, int x, int y, int value);
int bc_getbigcharpos (unsigned int *big, int x, int y, int *value);
int bc_bigcharwrite (int fd, int *big, int count);
int bc_bigcharread (int fd, int *big, int need_count, int *count);

#endif // MY_BIG_CHARS_H