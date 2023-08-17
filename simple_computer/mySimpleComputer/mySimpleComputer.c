#include <mySimpleComputer.h>

const uint8_t commands[NUM_COMMANDS] = { 0x10, 0x11, 0x20, 0x21, 0x30, 0x31,
                                         0x32, 0x33, 0x40, 0x41, 0x42, 0x43, 0x57};

/*
0x1000 = 2048
0x1100 = 2176
0x2000 = 4096
0x2100 = 4224
0x3000 = 6144
0x3100 = 6272
0x3200 = 6400
0x3300 = 6528
0x4000 = 8192
0x4100 = 8320
0x4200 = 8448
0x4300 = 8576
0x5700 = 11136
*/

short int *memory;
uint8_t reg_flags;
short int accumulator = 0;
uint8_t instructionCounter = 0;
short int operation;

int
sc_memoryInit ()
{
  memory = calloc (MEM_SIZE, sizeof (int));

  if (memory)
    return 0;
  else
    return -1;
}

int
sc_memorySet (int address, int value)
{
  if (address >= MEM_SIZE || address < 0)
    {
      reg_flags = reg_flags | OVERFLOW_MEMORY;
      return -1;
    }
  else
    {
      memory[address] = value;
      return 0;
    }
}

int
sc_memoryGet (int address, int *value)
{
  if (!value)
    {
      return -1;
    }

  if (address >= MEM_SIZE)
    {
      reg_flags = reg_flags | OVERFLOW_MEMORY;
      return -1;
    }
  else
    {
      *value = memory[address];
      return 0;
    }
}

int
sc_memorySave (char *filename)
{
  FILE *file = fopen (filename, "wb");

  if (!file)
    {
      return -1;
    }

  fwrite (memory, sizeof (int), MEM_SIZE, file);

  fclose (file);

  return 0;
}

int
sc_memoryLoad (char *filename)
{
  FILE *file = fopen (filename, "rb");

  if (!file)
    {
      return -1;
    }

  fread (memory, sizeof (int), MEM_SIZE, file);

  fclose (file);

  return 0;
}

int
sc_regInit ()
{
  reg_flags = 1 << 3;

  return 0;
}

int
sc_regSet (int register_, int value)
{
  if (register_ > 0 && register_ <= 5)
    {
      if (value == 1)
        reg_flags = reg_flags | (1 << (register_ - 1));
      else if (value == 0)
        reg_flags = reg_flags & ~(1 << (register_ - 1));
      else
        return -1;
      return 0;
    }
  else
    return -1;
}

int
sc_regGet (int register_, int *value)
{
  if (register_ > 0 && register_ <= 5)
    {
      *value = reg_flags >> (register_ - 1) & 1;
      return 0;
    }
  else
    return -1;
}

int
sc_commandEncode (int command, int operand, int *value)
{
  if (command > 0x7f || operand > 0x7f)
    return -1;

  *value = (command << 7) | operand;
  return 0;
}

int
sc_commandDecode (int value, int *command, int *operand)
{
  if (value >> 14)
    {
      return -1;
    }

  *command = value >> 7;
  *operand = value & 0x7F;

  return 0;
}