#ifndef CORE_LOOP_H // #define are called macros. Programmers use ALL_CAPS for macros
#define CORE_LOOP_H
#include <stdio.h>
#include <stdint.h>

//struct 
typedef struct {
    uint8_t ram[4096]; //8bits=1byte, 1byte x 4096bytes = 4kb
    uint16_t pc;
    uint16_t I;
    uint16_t stack[16];
    uint8_t sp; //stack pointer
    uint8_t dTimer;
    uint8_t sTimer;
    uint8_t V[16];
}chip8;


//prototypes of all functions delcare here, so any .c file including that header knows they exist and knows how to use them.
void file_op(char  *opcode);

#endif