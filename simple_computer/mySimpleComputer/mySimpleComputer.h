#ifndef MYSIMPLECOMPUTER_H
#define MYSIMPLECOMPUTER_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 100

extern short int *memory;
extern uint8_t reg_flags;
extern short int accumulator;
extern uint8_t instructionCounter;
extern short int operation;

/*
000 0000[1]  overflow when performing an operation 'P'
000000[1]0  a division error by 0 'O'
00000[1]00  overflow memory 'M'
0000[1]000  ignoring clock pulses 'T'
000[1]0000  an incorrect command 'E'
*/

enum mask_flags
{
  OVERFLOW_OPERATION = 1,
  DIV_BY_ZERO = OVERFLOW_OPERATION << 1,
  OVERFLOW_MEMORY = DIV_BY_ZERO << 1,
  IGNOR_CLOCK_PULSES = OVERFLOW_MEMORY << 1,
  INCORRECT_COMMAND = IGNOR_CLOCK_PULSES << 1
};

#define NUM_COMMANDS 13

int sc_memoryInit ();
int sc_memorySet (int address, int value);
int sc_memoryGet (int address, int *value);
int sc_memorySave (char *filename);
int sc_memoryLoad (char *filename);
int sc_regInit ();
int sc_regSet (int register_, int value);
int sc_regGet (int register_, int *value);
int sc_commandEncode (int command, int operand, int *value);
int sc_commandDecode (int value, int *command, int *operand);

/* func return 0 if execution was succsesfull, else func return -1*/

#endif // MYSIMPLECOMPUTER_H