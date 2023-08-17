#include <myBigChars.h>
#include <myReadkey.h>
#include <mySimpleComputer.h>
#include <myTerm.h>
#include <signal.h>
#include <sys/time.h>

int ALU (int command, int operand);
int CU ();

#define DEFAULT_X 0
#define DEFAULT_Y 0

int current_x, current_y;

#define TIME 1
struct itimerval val = { { 0, 100000 }, { 0, 100000 } }, last_val;

void
print_border ()
{
  bc_box (0, 0, 62, 12);
  bc_box (63, 1, 87, 3);
  bc_box (63, 4, 87, 6);
  bc_box (63, 7, 87, 9);
  bc_box (63, 10, 87, 12);
  bc_box (1, 13, 47, 22);
  bc_box (48, 13, 87, 22);
}

void
printCell (int address, colors bg_color, colors fg_color)
{
  mt_setbgcolor (bg_color);
  mt_setfgcolor (fg_color);

  int value, row, col, command, operand;
  char buf[6];
  if (sc_memoryGet (address, &value) < 0
      || sc_commandDecode (value & 0x3FFF, &command, &operand) < 0)
    return;

  row = address / 10;
  col = address % 10;

  snprintf (buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command,
            operand);

  mt_gotoXY (2 + col * 6, 2 + row);
  write (1, buf, 5);
  write (1, "\E[m", 3);
}

void
print_memory ()
{
  for (int i = 0; i < 100; i++)
    {
      printCell (i, BLACK, WHITE);
    }
}

void
print_accumulator ()
{
  mt_gotoXY (73, 2);
  char *str_accum = malloc (20);
  if (accumulator >> 14)
    {
      sprintf (str_accum, "-%04x", accumulator & 0x3fff);
    }
  else
    {
      sprintf (str_accum, "+%04x", accumulator);
    }
  write (1, str_accum, strlen (str_accum));

  free (str_accum);
}

void
print_instructionCounter ()
{
  mt_gotoXY (73, 5);
  char *instrCounter = malloc (20);
  sprintf (instrCounter, "+%04x", instructionCounter);
  write (1, instrCounter, strlen (instrCounter));

  free (instrCounter);
  mt_gotoXY (0, 25);
}

void
print_operation ()
{
  mt_gotoXY (64, 8);
  write (1, "                      ", 22);
  mt_gotoXY (71, 8);
  int value, command, operand;
  sc_memoryGet (instructionCounter, &value);
  sc_commandDecode (value & 0x3fff, &command, &operand);
  char *operation = malloc (20);
  sprintf (operation, "%c%02x : %02x", (value >> 14) ? '-' : '+', command,
           operand);
  write (1, operation, strlen (operation));

  free (operation);
  mt_gotoXY (0, 25);
}

void
print_flags ()
{
  mt_gotoXY (73, 11);
  char flags[] = "POMTE";
  int value = 0;
  for (int i = 0; i < 5; i++)
    {
      sc_regGet (i + 1, &value);
      if (value)
        {
          mt_setbgcolor (RED);
          write (1, &flags[i], 1);
          mt_setbgcolor (BLACK);
        }
      else
        {
          write (1, &flags[i], 1);
        }
    }
}

void
print_term_text ()
{
  mt_gotoXY (69, 1);
  write (1, " accumulator ", strlen (" accumulator "));
  mt_gotoXY (66, 4);
  write (1, " instructionCounter ", strlen (" instructionCounter "));
  mt_gotoXY (70, 7);
  write (1, " operation ", strlen (" operation "));
  mt_gotoXY (72, 10);
  write (1, " Flags ", strlen (" Flags "));
  mt_gotoXY (50, 13);
  write (1, " Keys: ", strlen (" Keys: "));
  mt_gotoXY (49, 14);
  write (1, "l  - load", strlen ("l  - load"));
  mt_gotoXY (49, 15);
  write (1, "s  - save", strlen ("s  - save"));
  mt_gotoXY (49, 16);
  write (1, "r  - run", strlen ("r  - run"));
  mt_gotoXY (49, 17);
  write (1, "t  - step", strlen ("t  - step"));
  mt_gotoXY (49, 18);
  write (1, "i  - reset", strlen ("i  - reset"));
  mt_gotoXY (49, 19);
  write (1, "q  - system shutdown", strlen ("q  - system shutdown"));
  mt_gotoXY (49, 20);
  write (1, "F5 - accumulator", strlen ("F5  - accumulator"));
  mt_gotoXY (49, 21);
  write (1, "F6 - instructionCounter", strlen ("F6  - instructionCounter"));
  mt_gotoXY (0, 24);
  write (1, "Input\\Output :", strlen ("Input\\Output :"));
  mt_gotoXY (28, 1);
  write (1, " memory ", strlen (" memory "));
}

void
print_terminal ()
{
  int cols, rows;
  mt_getscreensize (&rows, &cols);
  if (rows < 27)
    {
      write (1, "can't print terminal", strlen ("can't print terminal"));
      exit (1);
    }

  print_border ();
  print_term_text ();
  print_accumulator ();
  print_instructionCounter ();
  print_operation ();
  print_flags ();
  print_memory ();

  mt_gotoXY (0, rows);
}

void
print_big_char (int address)
{
  int value, command, operand;
  uint8_t buf;
  sc_memoryGet (address, &value);
  sc_commandDecode (value & 0x3fff, &command, &operand);
  if (value & 0x4000)
    bc_printbigchar (bch.minus, 2, 14, BLACK, YELLOW);
  else
    bc_printbigchar (bch.plus, 2, 14, BLACK, YELLOW);
  buf = (command >> 4) & 0x0f;
  bc_printbigchar (get_bigchar (buf), 11, 14, BLACK, YELLOW);
  buf = command & 0x0f;
  bc_printbigchar (get_bigchar (buf), 20, 14, BLACK, YELLOW);
  buf = (operand) >> 4 & 0x0f;
  bc_printbigchar (get_bigchar (buf), 29, 14, BLACK, YELLOW);
  buf = operand & 0x0f;
  bc_printbigchar (get_bigchar (buf), 38, 14, BLACK, YELLOW);
}

void
load_file ()
{
  char *load_file = malloc (20),
       empty_space[] = "                                    ";
  int size;
  rk_mytermsave ();
  rk_mytermregime (0, 0, 0, 1, 1);
  rk_mytermregime (1, 0, 0, 0, 0);
  mt_gotoXY (0, 25);
  write (1, "load_filename : ", strlen ("load_filename : "));
  size = read (1, load_file, 20);
  load_file[size - 1] = '\0';
  mt_gotoXY (0, 25);
  write (1, empty_space, strlen (empty_space));
  int flag = sc_memoryLoad (load_file);
  if (flag)
    {
      // write(1, "error", 5);
    }
  rk_mytermrestore ();
  system ("rm term_options");

  for (int i = 0; i < 100; i++)
    {
      printCell (i, BLACK, WHITE);
    }
  printCell (10 * current_y + current_x, WHITE, BLACK);
  print_operation ();
}

void
save_file ()
{
  char *save_file = malloc (20),
       empty_space[] = "                                    ";
  int size;
  rk_mytermsave ();
  rk_mytermregime (0, 0, 0, 1, 1);
  rk_mytermregime (1, 0, 0, 0, 0);
  mt_gotoXY (0, 25);
  write (1, "save_filename : ", strlen ("save_filename : "));
  size = read (1, save_file, 20);
  save_file[size - 1] = '\0';
  mt_gotoXY (0, 25);
  write (1, empty_space, strlen (empty_space));
  int flag = sc_memorySave (save_file);
  if (flag)
    {
      // write(1, "error", 5);
    }
  rk_mytermrestore ();
  system ("rm term_options");
}

void
reset_machine ()
{
  free (memory);
  sc_memoryInit ();
  sc_regInit ();
  print_flags ();
  print_memory ();
  print_operation ();
  current_x = DEFAULT_X, current_y = DEFAULT_Y;
  print_big_char (current_y * 10 + current_x);
  printCell (10 * current_y + current_x, WHITE, BLACK);
}

void
change_value (int x, int y)
{
  char *str_number = malloc (20),
       empty_space[] = "                              ";
  int size, number;
  rk_mytermsave ();
  rk_mytermregime (0, 0, 0, 1, 1);
  rk_mytermregime (1, 0, 0, 0, 0);
  mt_gotoXY (0, 25);
  write (1, "enter number : ", strlen ("enter number : "));
  size = read (1, str_number, 20);
  str_number[size] = '\0';
  number = atoi (str_number);
  mt_gotoXY (0, 25);
  write (1, empty_space, strlen (empty_space));

  if (number > 0x3fff)
    {
      mt_gotoXY (0, 25);
      write (1, "incorrect number! ", strlen ("incorrect number! "));
      for (int i = 5; i >= 0; i--)
        {
          char *num = malloc (1);
          sprintf (num, "%d ", i);
          ;
          write (1, num, 10);
          mt_gotoXY (19, 25);
          usleep (500000);
        }
      mt_gotoXY (0, 25);
      write (1, empty_space, strlen (empty_space));
      return;
    }
  if (number < 0)
    {
      number *= -1;
      if (number > 0x3fff)
        {
          mt_gotoXY (0, 25);
          write (1, "incorrect number! ", strlen ("incorrect number! "));
          for (int i = 5; i >= 0; i--)
            {
              char *num = malloc (1);
              sprintf (num, "%d ", i);
              ;
              write (1, num, 10);
              mt_gotoXY (19, 25);
              usleep (500000);
            }
          mt_gotoXY (0, 25);
          write (1, empty_space, strlen (empty_space));
          return;
        }
      number |= 1 << 14;
      mt_gotoXY (0, 25);
    }
  sc_memorySet (y * 10 + x, number);
  printCell(y * 10 + x, BLACK, WHITE);
  rk_mytermrestore ();
  system ("rm term_options");
}

void
set_accumulator ()
{
  char *str_number = malloc (20),
       empty_space[] = "                              ";
  int size, number;
  rk_mytermsave ();
  rk_mytermregime (0, 0, 0, 1, 1);
  rk_mytermregime (1, 0, 0, 0, 0);
  mt_gotoXY (0, 25);
  write (1, "enter acumulator : ", strlen ("enter acumulator : "));
  size = read (1, str_number, 20);
  str_number[size] = '\0';
  number = atoi (str_number);
  mt_gotoXY (0, 25);
  write (1, empty_space, strlen (empty_space));

  if (number > 0x3fff)
    {
      mt_gotoXY (0, 25);
      write (1, "incorrect number! ", strlen ("incorrect number! "));
      for (int i = 5; i >= 0; i--)
        {
          char *num = malloc (1);
          sprintf (num, "%d ", i);
          ;
          write (1, num, 10);
          mt_gotoXY (19, 25);
          usleep (500000);
        }
      mt_gotoXY (0, 25);
      write (1, empty_space, strlen (empty_space));
      return;
    }
  if (number < 0)
    {
      number *= -1;
      if (number > 0x3fff)
        {
          mt_gotoXY (0, 25);
          write (1, "incorrect number! ", strlen ("incorrect number! "));
          for (int i = 5; i >= 0; i--)
            {
              char *num = malloc (1);
              sprintf (num, "%d ", i);
              ;
              write (1, num, 10);
              mt_gotoXY (19, 25);
              usleep (500000);
            }
          mt_gotoXY (0, 25);
          write (1, empty_space, strlen (empty_space));
          return;
        }
      number |= 1 << 14;
      mt_gotoXY (0, 25);
      printf ("%d\n", number);
    }
  accumulator = number;
  print_accumulator (number);
  rk_mytermrestore ();
  system ("rm term_options");
}

void
set_instructionCounter ()
{
  char *str_number = malloc (20),
       empty_space[] = "                              ";
  int size, number;
  rk_mytermsave ();
  rk_mytermregime (0, 0, 0, 1, 1);
  rk_mytermregime (1, 0, 0, 0, 0);
  mt_gotoXY (0, 25);
  write (1, "enter instructionCounter : ",
         strlen ("enter instructionCounter : "));
  size = read (1, str_number, 20);
  str_number[size] = '\0';
  number = atoi (str_number);
  mt_gotoXY (0, 25);
  write (1, empty_space, strlen (empty_space));

  if (number > 99 || number < 0)
    return;
  instructionCounter = number;
  print_operation ();
  print_instructionCounter ();
  rk_mytermrestore ();
  system ("rm term_options");
}

void
execute_press_command (keys key)
{
  switch (key)
    {
    case KEY_l:
      load_file ();
      break;
    case KEY_s:
      save_file ();
      break;
    case KEY_r:
      print_operation ();
      setitimer (ITIMER_REAL, &val, &last_val);
      sc_regSet (4, 0);
      print_flags ();
      break;
    case KEY_t:
      CU ();
      break;
    case KEY_i:
      reset_machine ();
      break;
    case KEY_f5:
      set_accumulator ();
      break;
    case KEY_f6:
      set_instructionCounter ();
      break;
    case KEY_up:
      if (current_y > 0)
        {
          printCell (10 * current_y + current_x, BLACK, WHITE);
          current_y--;
          printCell (10 * current_y + current_x, WHITE, BLACK);
          print_big_char (current_y * 10 + current_x);
        }
      else
        {
          printCell (10 * current_y + current_x, BLACK, WHITE);
          current_y = 9;
          printCell (10 * current_y + current_x, WHITE, BLACK);
          print_big_char (current_y * 10 + current_x);
        }
      break;
    case KEY_down:
      if (current_y < 9)
        {
          printCell (10 * current_y + current_x, BLACK, WHITE);
          current_y++;
          printCell (10 * current_y + current_x, WHITE, BLACK);
          print_big_char (current_y * 10 + current_x);
        }
      else
        {
          printCell (10 * current_y + current_x, BLACK, WHITE);
          current_y = 0;
          printCell (10 * current_y + current_x, WHITE, BLACK);
          print_big_char (current_y * 10 + current_x);
        }
      break;
    case KEY_right:
      if (current_x < 9)
        {
          printCell (10 * current_y + current_x, BLACK, WHITE);
          current_x++;
          printCell (10 * current_y + current_x, WHITE, BLACK);
          print_big_char (current_y * 10 + current_x);
        }
      else
        {
          printCell (10 * current_y + current_x, BLACK, WHITE);
          current_x = 0;
          printCell (10 * current_y + current_x, WHITE, BLACK);
          print_big_char (current_y * 10 + current_x);
        }
      break;
    case KEY_left:
      if (current_x > 0)
        {
          printCell (10 * current_y + current_x, BLACK, WHITE);
          current_x--;
          printCell (10 * current_y + current_x, WHITE, BLACK);
          print_big_char (current_y * 10 + current_x);
        }
      else
        {
          printCell (10 * current_y + current_x, BLACK, WHITE);
          current_x = 9;
          printCell (10 * current_y + current_x, WHITE, BLACK);
          print_big_char (current_y * 10 + current_x);
        }
      break;
    case KEY_enter:
      change_value (current_x, current_y);
      printCell (current_y * 10 + current_x, WHITE, BLACK);
      print_big_char (current_y * 10 + current_x);
      break;
    case KEY_q:
      rk_mytermregime (0, 0, 0, 1, 1);
      rk_mytermregime (1, 0, 0, 0, 0);
      write (1, "\E[?25h\E[?8c", strlen ("\E[?25h\E[?8c"));
      mt_gotoXY (0, 25);
      exit (0);
      break;
    case KEY_other:
      break;
    }
}

int
ALU (int command, int operand)
{
  int value = 0;
  switch (command)
    {

    case 0x30: // ADD  Выполняет сложение слова в аккумуляторе и слова из
               // указанной ячейки памяти
      sc_memoryGet (operand, &value);
      value &= 0x3fff;
      if (accumulator + value > 0x3fff)
        {
          sc_regSet (1, 1);
          print_flags ();
          return -1;
        }
      accumulator += value;
      print_accumulator ();
      break;

    case 0x31: // SUB Вычитает из слова в аккумуляторе слово из указанной
               // ячейки памяти
      sc_memoryGet (operand, &value);
      // value &= 0x3fff;
      if (accumulator - value < -0x3fff)
        {
          sc_regSet (1, 1);
          print_flags ();
          return -1;
        }
      accumulator -= value;
      if (accumulator < 0) {
        accumulator = ~accumulator + 1;
        accumulator |= 1 << 14; 
      }
      print_accumulator ();
      break;

    case 0x32: // DIVIDE Выполняет деление слова в аккумуляторе на слово из
               // указанной ячейки памяти
      sc_memoryGet (operand, &value);
      value &= 0x3fff;
      if (value == 0)
        {
          sc_regSet (2, 1);
          print_flags ();
          return -1;
        }
      accumulator /= value;
      print_accumulator ();
      break;

    case 0x33: // MUL Вычисляет произведение слова в аккумуляторе на слово из
               // указанной ячейки памяти
      sc_memoryGet (operand, &value);
      value &= 0x3fff;
      if (accumulator * value > 0x3fff || accumulator * value < -0x3fff)
        {
          sc_regSet (1, 1);
          print_flags ();
          return -1;
        }
      accumulator *= value;
      print_accumulator ();
      break;
    }

  return 0;
}

int
CU ()
{
  int value, command, operand;

  if (instructionCounter > 98)
    {
      alarm (0);
      sc_regSet (4, 1);
      print_flags ();
      printCell (instructionCounter, BLACK, WHITE);
      printCell (current_y * 10 + current_x, WHITE, BLACK);
      return -1;
    }

  printCell (instructionCounter, BLACK, WHITE);
  sc_memoryGet (instructionCounter++, &value);
  printCell (instructionCounter, WHITE, BLACK);

  if (sc_commandDecode (value, &command, &operand) == -1)
    {
      alarm (0);
      sc_regSet (4, 1);
      sc_regSet (5, 1);
      print_flags ();
      return -1;
    }

  if (command >= 0x30 && command <= 0x33)
    {
      ALU (command, operand);
    }
  else
    {
      switch (command)
        {
        case 0x10: // READ Ввод с терминала в указанную ячейку памяти с
                   // контролем переполнения
          change_value (operand % 10, operand / 10);
          break;

        case 0x11: // WRITE Загрузка в аккумулятор значения из указанного
                   // адреса памяти
          print_big_char (operand);
          break;

        case 0x20: // LOAD Загрузка в аккумулятор значения из указанного адреса
                   // памяти
          if (!sc_memoryGet (operand, &value))
            {
              accumulator = value;
              print_accumulator ();
            }
          break;

        case 0x21: // STORE Выгружает значение из аккумулятора по указанному
                   // адресу памяти
          if (!sc_memorySet (operand, accumulator)) {
            printCell (operand, BLACK, WHITE);
          }

          break;

        case 0x40: // JUMP Переход к указанному адресу памяти
          if (operand >= 0 || operand <= 99)
            {
              printCell (instructionCounter, BLACK, WHITE);
              instructionCounter = operand;
              printCell (instructionCounter, WHITE, BLACK);
            }
          break;

        case 0x41: // JNEG Переход к указанному адресу памяти, если в
                   // аккумуляторе находится отрицательное число
          if (accumulator >> 14)
            {
              if (operand >= 0 || operand <= 99)
                {
                  printCell (instructionCounter, BLACK, WHITE);
                  instructionCounter = operand;
                  printCell (instructionCounter, WHITE, BLACK);
                }
            }
          break;

        case 0x42: // JZ Переход к указанному адресу памяти, если в
                   // аккумуляторе находится ноль
          if (accumulator == 0)
            {
              if (operand >= 0 || operand <= 99)
                {
                  printCell (instructionCounter, BLACK, WHITE);
                  instructionCounter = operand;
                  printCell (instructionCounter, WHITE, BLACK);
                }
            }
          break;

        case 0x43: // HALT Останов, выполняется при завершении работы программы
          alarm (0);
          sc_regSet (4, 1);
          print_flags ();
          printCell (instructionCounter, BLACK, WHITE);
          printCell (current_y * 10 + current_x, WHITE, BLACK);
          break;

        case 0x57: // JNC  Переход к указанному адресу памяти, если при
                   // сложении не произошло переполнение
          sc_regGet (1, &value);
          if (!value)
            {
              printCell (instructionCounter, BLACK, WHITE);
              instructionCounter = operand;
              printCell (instructionCounter, WHITE, BLACK);
            }
          break;
        }
    }
  print_operation ();
  print_instructionCounter ();
  return 0;
}

void
timeout (int signal)
{
  CU ();
}

int
main ()
{
  write (1, "\E[?25l\E[?1c", strlen ("\E[?25l\E[?1c"));
  mt_clrscr ();

  if (!isatty (1))
    {
      fprintf (stderr, "error");
      exit (0);
    }

  sc_memoryInit ();
  sc_regInit ();
  bc_init_big_chars ();

  print_terminal ();

  mt_gotoXY (DEFAULT_X, DEFAULT_Y);
  printCell (10 * current_y + current_x, WHITE, BLACK);
  print_big_char (current_y * 10 + current_x);
  current_x = DEFAULT_X, current_y = DEFAULT_Y;
  keys key;

  signal (SIGALRM, timeout);
  signal (SIGUSR1, reset_machine);
  int value = 0;
  print_flags ();
  while (1)
    {
      sc_regGet (4, &value);
      if (value)
        {
          rk_readkey (&key);
          execute_press_command (key);
        }
    }

  return 0;
}