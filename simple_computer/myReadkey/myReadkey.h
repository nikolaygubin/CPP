#ifndef MY_READKEY_H
#define MY_READKEY_H

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

typedef enum keys
{
  KEY_l,     // l
  KEY_s,     // s
  KEY_r,     // r
  KEY_t,     // t
  KEY_i,     // i
  KEY_f5,    // \E[15~
  KEY_f6,    // \E[17~
  KEY_up,    // \E[A
  KEY_down,  // \E[B
  KEY_right, // \E[C
  KEY_left,  // \E[D
  KEY_enter, // \n
  KEY_q,     // q
  KEY_other
} keys;

int rk_readkey (keys *key);
int rk_mytermsave ();
int rk_mytermrestore ();
int rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint);

#endif