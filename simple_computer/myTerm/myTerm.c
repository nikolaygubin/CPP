#include <myTerm.h>

int
mt_clrscr ()
{
  char str[] = "\E[H\E[J";
  write (1, str, strlen (str)); //(поток, буффер, размер буффера)

  return 0;
}

int
mt_gotoXY (int x, int y)
{
  int *cols = malloc (4), *rows = malloc (4);

  mt_getscreensize (rows, cols);

  if (x < 0 || x > *cols || y < 0 || y > *rows)
    {
      free (cols);
      free (rows);
      return -1;
    }

  free (cols);
  free (rows);

  char *str = malloc (30);
  sprintf (str, "\E[%d;%dH", y, x);
  write (1, str, strlen (str));

  return 0;
}

int
mt_getscreensize (int *rows, int *cols)
{
  struct winsize ws;

  if (!ioctl (1, TIOCGWINSZ, &ws))
    {
      *rows = ws.ws_row;
      *cols = ws.ws_col;
      return 0;
    }
  else
    {
      return -1;
    }
  return 0;
}

int
mt_setfgcolor (colors color)
{
  char *str = malloc (30);
  sprintf (str, "\E[3%dm", color);
  write (1, str, strlen (str));

  return 0;
}

int
mt_setbgcolor (colors color)
{
  char *str = malloc (30);
  sprintf (str, "\E[4%dm", color);
  write (1, str, strlen (str));

  return 0;
}