#include <myReadkey.h>
#include <mySimpleComputer.h>
#include <myTerm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int
main ()
{
  sc_memoryInit ();
  sc_regInit ();

  keys key;
  for (int i = 0; i < 10; i++)
    {
      rk_readkey (&key);
      switch (key)
        {
        case KEY_q:
          break;
        case KEY_l:
          break;

        case KEY_s:
          break;

        case KEY_r:
          rk_mytermsave ();
          break;

        case KEY_t:
          write (1, "\E[?25l\E[?1c",
                 strlen ("\E[?25l\E[?1c")); // cursoor unvisiable
          break;

        case KEY_i:
          write (1, "\E[?25h\E[?8c",
                 strlen ("\E[?25h\E[?8c")); // cursor visiable
          break;

        case KEY_f5:
          break;

        case KEY_f6:
          break;

        case KEY_up:
          write (1, "\E[A", strlen ("\E[A"));
          break;

        case KEY_down:
          write (1, "\E[B", strlen ("\E[B"));
          break;

        case KEY_right:
          write (1, "\E[C", strlen ("\E[C"));
          break;

        case KEY_left:
          write (1, "\E[D", strlen ("\E[D"));
          break;

        case KEY_enter:
          write (1, "\n", 1);
          break;

        case KEY_other:
          break;
        }
    }

  rk_mytermregime (0, 0, 0, 1, 1);

  rk_mytermregime (1, 0, 0, 0, 0);

  return 0;
}