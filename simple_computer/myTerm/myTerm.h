#ifndef MY_TERM_H
#define MY_TERM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

typedef enum colors
{
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGNETA,
  CYAN,
  WHITE
} colors;

int mt_clrscr ();
int mt_gotoXY (int x, int y);
int mt_getscreensize (int *rows, int *cols);
int mt_setfgcolor (colors color);
int mt_setbgcolor (colors color);

#endif // MY_TERM_H