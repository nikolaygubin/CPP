#include <mySimpleComputer.h>

int
main ()
{
  int *value = malloc (4);

  sc_memoryInit ();
  sc_regInit ();

  sc_memorySet (23, 12525);
  sc_memorySet (105, 65);
  sc_regGet (3, value);
  printf ("OVERFLOW_MEMORY : %d\n", *value);
  sc_memorySet (19, 17);

  sc_memoryGet (23, value);
  printf ("memory[23] =  %d\n", *value);
  sc_memoryGet (105, value);
  printf ("memory[105] =  %d\n", *value);
  sc_memoryGet (19, value);
  printf ("memory[17] =  %d\n", *value);

  sc_memorySave (
      "save_file.b"); // сохраняем оперативную память в бинарном виде

  sc_memorySet (19, 177); // меняем зачение по адресу 19
  sc_memoryGet (19, value);
  printf ("memory[17] =  %d\n", *value);

  sc_memoryLoad (
      "save_file.b"); // подгружаем из файла данные в оперативную память ->
                      // зачение по адресу 19 снова изменилось
  sc_memoryGet (19, value);
  printf ("memory[17] =  %d\n", *value);

  sc_regGet (3, value);
  printf ("value in 3 register = %d\n", *value);

  sc_regSet (3, 1);
  sc_regGet (3, value);
  printf ("value in 3 register = %d\n", *value);

  sc_regSet (3, 0);
  sc_regGet (3, value);
  printf ("value in 3 register = %d\n", *value);

  sc_regSet (7, 1);
  sc_regGet (7, value);
  printf ("value in 7 register = %d\n", *value);

  int *command = malloc (4), *operand = malloc (4);

  sc_commandEncode (10, 45, value);
  sc_commandDecode (*value, command, operand);
  printf ("command = %d\noperand = %d\n", *command, *operand);

  sc_commandEncode (108, 66, value); // зачение command и operand не изменилось
                                     // так как введена неизвестная команда
  sc_commandDecode (*value, command, operand);
  printf ("command = %d\noperand = %d\n", *command, *operand);

  return 0;
}