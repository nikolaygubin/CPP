#include <myBigChars.h>
#include <myTerm.h>
#include <time.h>

int
main ()
{
  mt_clrscr ();
  bc_init_big_chars ();

  int posx = 2, pos_one = 2;
  for (int i = 0; i < 16; i++)
    {
      bc_box (posx - 2, 0, posx + 9, 10);
      bc_box (posx - 2, 11, posx + 9, 21);
      bc_printbigchar (get_bigchar (i), posx, 2, BLACK, BLUE);
      posx += 12;
    }

  posx = 2;
  for (int i = 0; i < 17; i++)
    {
      bc_printbigchar (bch.zero, posx, 13, BLACK, BLUE);
      posx = 2 + i * 12;
    }

  while (1)
    {
      bc_printbigchar (bch.one, pos_one, 13, BLACK, BLUE);
      if (pos_one - 12 < 0)
        bc_printbigchar (bch.zero, 2 + 12 * 15, 13, BLACK, BLUE);
      else
        bc_printbigchar (bch.zero, pos_one - 12, 13, BLACK, BLUE);
      pos_one += 12;
      if (pos_one == 2 + 12 * 16)
        pos_one = 2;
      usleep (500000);
    }

  mt_gotoXY (0, 23);

  return 0;
}