#include <myBigChars.h>

// unsigned int zero[2]
//     = { 0x3c428181, 0x8181423c },
//     one[2] = { 0x3878d818, 0x1818187e }, two[2] = { 0x7ec30106, 0x186080ff
//     }, three[2] = { 0xff02040f, 0x0101817e }, four[2] = { 0x0e122242,
//     0xff020202 }, five[2] = { 0xff8080ff, 0x010101ff }, six[2] = {
//     0x183060c0, 0xff8181ff }, seven[2] = { 0xff03060c, 0x183060c0 },
//     eight[2] = { 0xff818181, 0xff8181ff },
//     nine[2] = { 0xff8181ff, 0x03060c18 }, A[2] = { 0x3c66c381, 0xff818181 },
//     B[2] = { 0xfcc1c1c1, 0xfec1c1fe }, C[2] = { 0x3fc0c0c0, 0xc0c0c03f },
//     D[2] = { 0xfec1c1c1, 0xc1c1c1fe }, E[2] = { 0xffc0c0c0, 0xffc0c0ff },
//     F[2] = { 0xffc0c0c0, 0xfcc0c0c0 }, plus[2] = { 0x181818ff, 0xff181818 },
//     minus[2] = { 0x000000ff, 0xff000000 };

struct BigChars bch;

void
bc_init_big_chars ()
{
  bch.zero[0] = 0x3c428181, bch.zero[1] = 0x8181423c;
  bch.one[0] = 0x3878d818, bch.one[1] = 0x1818187e;
  bch.two[0] = 0x7ec30106, bch.two[1] = 0x186080ff;
  bch.three[0] = 0xff02040f, bch.three[1] = 0x0101817e;
  bch.four[0] = 0x0e122242, bch.four[1] = 0xff020202;
  bch.five[0] = 0xff8080ff, bch.five[1] = 0x010101ff;
  bch.six[0] = 0x183060c0, bch.six[1] = 0xff8181ff;
  bch.seven[0] = 0xff03060c, bch.seven[1] = 0x183060c0;
  bch.eight[0] = 0xff818181, bch.eight[1] = 0xff8181ff;
  bch.nine[0] = 0xff8181ff, bch.nine[1] = 0x03060c18;
  bch.A[0] = 0x3c66c381, bch.A[1] = 0xff818181;
  bch.B[0] = 0xfec1c1c1, bch.B[1] = 0xfec1c1fe;
  bch.C[0] = 0x3fc0c0c0, bch.C[1] = 0xc0c0c03f;
  bch.D[0] = 0xfec1c1c1, bch.D[1] = 0xc1c1c1fe;
  bch.E[0] = 0xffc0c0c0, bch.E[1] = 0xffc0c0ff;
  bch.F[0] = 0xffc0c0c0, bch.F[1] = 0xfcc0c0c0;
  bch.plus[0] = 0x181818ff, bch.plus[1] = 0xff181818;
  bch.minus[0] = 0x000000ff, bch.minus[1] = 0xff000000;
}

unsigned int *
get_bigchar (uint8_t buf)
{
  switch (buf)
    {
    case 0x0:
      return bch.zero;
      break;
    case 0x1:
      return bch.one;
      break;
    case 0x2:
      return bch.two;
      break;
    case 0x3:
      return bch.three;
      break;
    case 0x4:
      return bch.four;
      break;
    case 0x5:
      return bch.five;
      break;
    case 0x6:
      return bch.six;
      break;
    case 0x7:
      return bch.seven;
      break;
    case 0x8:
      return bch.eight;
      break;
    case 0x9:
      return bch.nine;
      break;
    case 0xa:
      return bch.A;
      break;
    case 0xb:
      return bch.B;
      break;
    case 0xc:
      return bch.C;
      break;
    case 0xd:
      return bch.D;
      break;
    case 0xe:
      return bch.E;
      break;
    case 0xf:
      return bch.F;
      break;
    default:
      return NULL;
    }
}

int
bc_printA (char *str)
{
  if (!str)
    return -1;

  char *string = malloc (30);
  sprintf (string, "%s%s%s", "\E(0", str, "\E(B");
  write (1, string, strlen (string));
  free (string);

  return 0;
}

int
bc_box (int x1, int y1, int x2, int y2)
{
  int cols, rows;
  mt_getscreensize (&rows, &cols);

  if (y1 > rows || y2 > rows || x1 > cols || x2 > cols || x1 < 0 || x2 < 0
      || y1 < 0 || y2 < 0)
    return -1;

  for (int i = x1 + 1; i < x2; i++)
    {
      mt_gotoXY (i, y1);
      bc_printA ("q");
      mt_gotoXY (i, y2);
      bc_printA ("q");
    }

  for (int i = y1 + 1; i <= y2; i++)
    {
      mt_gotoXY (x1, i);
      bc_printA ("x");
      mt_gotoXY (x2, i);
      bc_printA ("x");
    }

  mt_gotoXY (x1, y1);
  bc_printA ("l");

  mt_gotoXY (x2, y1);
  bc_printA ("k");

  mt_gotoXY (x2, y2);
  bc_printA ("j");

  mt_gotoXY (x1, y2);
  bc_printA ("m");

  return 0;
}

int
bc_printbigchar (unsigned int arr[2], int x, int y, colors back_color,
                 colors fore_color)
{
  int rows, cols;
  mt_getscreensize (&rows, &cols);
  if (x < 0 || y < 0 || x > cols || y > rows)
    return -1;

  mt_setbgcolor (back_color);
  mt_setfgcolor (fore_color);

  int8_t buf;
  int64_t num = ((int64_t)arr[0] << 32) | arr[1];
  for (int i = 7; i >= 0; i--)
    {
      mt_gotoXY (x, y + i);
      buf = num;
      for (int j = 0; j < 8; j++)
        {
          if ((buf >> (7 - j)) & 1)
            {
              bc_printA ("a");
            }
          else
            {
              bc_printA (" ");
            }
        }
      num >>= 8;
    }
  mt_setbgcolor (BLACK);
  mt_setfgcolor (WHITE);

  return 0;
}

int
bc_setbigcharpos (unsigned int *big, int x, int y, int value)
{
  if ((value != 0 && value != 1) || x < 1 || x > 8 || y < 1 || y > 8)
    return -1;
  y--, x--;

  int index = y / 4, place = y % 4, num = 1;
  num <<= ((4 - place) * 8 - x - 1);

  if (value)
    {
      big[index] |= num;
    }
  else
    {
      big[index] &= ~num;
    }

  return 0;
}

int
bc_getbigcharpos (unsigned int *big, int x, int y, int *value)
{
  if (x < 1 || x > 8 || y < 1 || y > 8)
    return -1;

  y--, x--;

  int num = (y / 4) ? big[1] : big[0];
  *value = (num >> ((4 - y % 4) * 8 - x - 1)) & 1;

  return 0;
}

int
bc_bigcharwrite (int fd, int *big, int count)
{
  return write (fd, big, count * 2 * sizeof (int));
}

int
bc_bigcharread (int fd, int *big, int need_count, int *count)
{

  if ((*count = read (fd, big, need_count * 2 * sizeof (int))) == 0)
    return -1;

  *count /= sizeof (int) * 2;

  return 0;
}