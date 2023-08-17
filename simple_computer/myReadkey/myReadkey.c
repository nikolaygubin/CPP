#include <myReadkey.h>

int
rk_readkey (keys *key)
{
  char buf[6];

  rk_mytermregime (0, 0, 1, 0, 0);
  for (int i = 0; i < 5; i++)
    {
      read (1, buf + i, 1);
      buf[i + 1] = '\0';

      if (!strcmp (buf, "l"))
        {
          *key = KEY_l;
          break;
        }
      else if (!strcmp (buf, "s"))
        {
          *key = KEY_s;
          break;
        }
      else if (!strcmp (buf, "r"))
        {
          *key = KEY_r;
          break;
        }
      else if (!strcmp (buf, "t"))
        {
          *key = KEY_t;
          break;
        }
      else if (!strcmp (buf, "i"))
        {
          *key = KEY_i;
          break;
        }
      else if (!strcmp (buf, "\n"))
        {
          *key = KEY_enter;
          break;
        }
      else if (!strcmp (buf, "q"))
        {
          *key = KEY_q;
          break;
        }
      else if (!strcmp (buf, "\E[15~"))
        {
          *key = KEY_f5;
          break;
        }
      else if (!strcmp (buf, "\E[17~"))
        {
          *key = KEY_f6;
          break;
        }
      else if (!strcmp (buf, "\E[A"))
        {
          *key = KEY_up;
          break;
        }
      else if (!strcmp (buf, "\E[B"))
        {
          *key = KEY_down;
          break;
        }
      else if (!strcmp (buf, "\E[C"))
        {
          *key = KEY_right;
          break;
        }
      else if (!strcmp (buf, "\E[D"))
        {
          *key = KEY_left;
          break;
        }
      else
        {
          int wrong_sequence = 0;
          switch (i)
            {
            case 0:
              if (buf[i] != '\E')
                wrong_sequence = 1;
              break;
            case 1:
              if (buf[i] != '[')
                wrong_sequence = 1;
              break;
            case 2:
              if (buf[i] != '1' && buf[i] != 'A' && buf[i] != 'B'
                  && buf[i] != 'C' && buf[i] != 'D')
                wrong_sequence = 1;
              break;
            case 3:
              if (buf[i] != '5' && buf[i] != '7')
                wrong_sequence = 1;
              break;
            case 4:
              if (buf[i] != '~')
                wrong_sequence = 1;
              break;
            }
          if (wrong_sequence)
            {
              *key = KEY_other;
              break;
            }
        }
    }

  rk_mytermregime (1, 0, 0, 0, 0);

  return 0;
}

int
rk_mytermsave ()
{
  struct termios options;
  if (tcgetattr (1, &options))
    {
      return -1;
    }

  FILE *save;
  if ((save = fopen ("term_options", "wb")) == NULL)
    {
      return -1;
    }
  else
    {
      if (fwrite (&options, sizeof (struct termios), 1, save) <= 0)
        return -1;
    }

  return 0;
}

int
rk_mytermrestore ()
{
  struct termios options;

  FILE *store;
  if ((store = fopen ("term_options", "rb")) == NULL)
    {
      fprintf (stderr, "Can't open file!");
      return -1;
    }
  else
    {
      if (fread (&options, sizeof (struct termios), 1, store) <= 0)
        return -1;
    }
  if (tcsetattr (1, TCSANOW, &options))
    return -1;

  return 0;
}

int
rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint)
{
  struct termios options;
  if (tcgetattr (1, &options))
    {
      return -1;
    }

  if (regime)
    options.c_lflag |= ICANON;
  else
    {
      options.c_lflag &= ~ICANON;
      options.c_cc[VTIME] = vtime;
      options.c_cc[VMIN] = vmin;
      if (echo)
        {
          options.c_lflag |= ECHO;
        }
      else
        {
          options.c_lflag &= ~ECHO;
        }
      if (sigint)
        {
          options.c_lflag |= ISIG;
        }
      else
        {
          options.c_lflag &= ~ISIG;
        }
    }
  tcsetattr (1, TCSAFLUSH, &options);

  return 0;
}