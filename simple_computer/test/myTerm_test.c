#include <mySimpleComputer.h>
#include <myTerm.h>
#include <stdlib.h>
#include <time.h>

int
main ()
{
  int *rows = malloc (4), *cols = malloc (4);

  mt_getscreensize (rows, cols);
  mt_clrscr ();

  int x, y;
  x = rand () % *cols, y = rand () % *rows;
  char *str1 = " Я ОЧЕНЬ", *str2 = " СИЛЬНО ЛЮБЛЮ ",
       *str3 = " АРХИТЕКТУРУ ЭВМ ";
  srand (time (NULL));
  for (int i = 0; i < 30; i++)
    {
      mt_gotoXY (x, y);
      mt_setfgcolor (GREEN);
      mt_setbgcolor (RED);
      write (1, str1, strlen (str1));
      x = rand () % *cols, y = rand () % *rows;

      mt_gotoXY (x, y);
      mt_setfgcolor (YELLOW);
      mt_setbgcolor (WHITE);
      write (1, str2, strlen (str2));
      x = rand () % *cols, y = rand () % *rows;

      mt_gotoXY (x, y);
      mt_setfgcolor (WHITE);
      mt_setbgcolor (MAGNETA);
      write (1, str3, strlen (str3));
      x = rand () % *cols, y = rand () % *rows;

      mt_setbgcolor (BLACK);
      mt_setfgcolor (WHITE);

      // mt_clrscr();
    }

  mt_gotoXY (0, *rows);

  putchar ('\n');
  printf ("rows = %d\ncols = %d\n", *rows, *cols);

  return 0;
}