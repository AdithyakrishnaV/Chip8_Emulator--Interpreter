#include <stdio.h>
#include <stdlib.h> 
#include "core_loop.h"
#include <stdint.h>  

//code
void file_op(char* file, chip8 *ch){
    // printf("my-c file path working argv,%s \n", opcode);
    FILE *opcode_file = fopen(file,"rb");

    if(! opcode_file){ printf("Couldn't open file\n"); return; } 

    //directly read to memory
    // size_t rom_file = fread(&ch->ram[0x200],sizeof(uint8_t),3584,opcode_file);
    fread(&ch->ram[0x200],sizeof(uint8_t),3584,opcode_file);
    int t=1;
    while(t==1){
        // Fetch: Combine two consecutive 8-bit RAM bytes into one 16-bit opcode (Big-Endian)
        uint16_t opcode = (ch->ram[ch->pc] <<8) | ch->ram[ch->pc+1];
        printf("mc- opcode:%x\n",opcode);
        t=0;
        //pc+=2;//calculation done in bytes so 2 byte = 16 bits
    }

    fclose(opcode_file);

}
//  fread(buffer, sizeof(int),5, file);
