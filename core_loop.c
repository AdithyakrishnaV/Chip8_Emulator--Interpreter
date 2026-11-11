#include <stdio.h>
#include <stdlib.h> 
#include "core_loop.h"
#include <stdint.h>  
#include <string.h>

//code
void file_op(char* file, chip8 *ch){
    // printf("my-c file path working argv,%s \n", opcode);
    FILE *opcode_file = fopen(file,"rb");

    if(! opcode_file){ printf("Couldn't open file\n"); return; } 

    //directly read to memory
    fread(&ch->ram[0x200],sizeof(uint8_t),3584,opcode_file);

    int t=1;
    while(t==1){
        // Fetch: Combine two consecutive 8-bit RAM bytes into one 16-bit opcode (Big-Endian)
        uint16_t opcode = (ch->ram[ch->pc] <<8) | ch->ram[ch->pc+1];
        uint16_t type   =  opcode >> 12;

        uint16_t nnn= (opcode & 0x0fff);//bit masking
        uint8_t   nn= (opcode &0xff);
        uint8_t    n= (opcode & 0xf);
        uint8_t    x= (opcode >> 8)  &0xf;
        uint8_t    y= (opcode >> 4)  &0xf;
        //printf("mc- opcode:%x\n",opcode);

        switch(type){
            case 0x0:
                switch(nn){
                // case nnn:
                //     break;
                    case 0xE0:
                        memset(ch->display, 0, sizeof(ch->display));
                        break;
                    case 0xEE:
                        ch->pc= ch->stack[ch->sp];//sp pointes to top of stack
                        ch->sp--;
                        break;
                }
                break;
            
            case 0x1: 
                ch->pc=nnn;
                break;
            
            case 0x2:
                ch->sp++;
                ch->stack[ch->sp]=ch->pc;
                ch->pc=nnn;
                break;
            
            case 0x3://Note that X and Y are always used to look up the values in registers.
                if(ch->V[x] == nn){
                    ch->pc +=2;//skip to next instruction
                }
                break;
            case 0x4:
                if(ch->V[x]!=nn){
                    ch->pc+=2;
                }
                break;
            
            case 0x5:
                //if Vx = Vy. increments the program counter by 2.
                if(ch->V[x]==ch->V[y]){
                    ch->pc+=2;
                }
                break;
            
            case 0x6:
                ch->V[x]= nn;
                break;

            case 0x7:
                ch->V[x]= ch->V[x] + nn;
                break;
            
            case 0x8:{
                switch(n){
                    case 0x0:
                        ch->V[x]= ch->V[y];
                        break;
                    case 0x1:
                        ch->V[x]= (ch->V[x] | ch->V[y]);
                        break;

                    case 0x3:
                        ch->V[x]= (ch->V[x] ^ ch->V[y]);
                        break;
                    
                    case 0x4:{
                        uint16_t sum = ch->V[x] + ch->V[y];
                        if(sum > 255){
                            ch->V[0xF]=1; // 0xF = 15
                        }else{
                            ch->V[0xF]=0;
                            }
                        ch->V[x]= sum & 0xff;
                        break;
                    }

                    case 0x5:{
                        if(ch->V[x] > ch->V[y]){
                            ch->V[0xF]=1;
                        }else{
                            ch->V[0xF]=0;
                        }
                        ch->V[x]= ch->V[x] - ch->V[y];
                        break;
                    }

                    case 0x6:{
                        ch->V[0xF]= ch->V[x] & 0x1; //0001 isolates the last bit or the least significant bit
                        ch->V[x] >>=1;             //ch->V[x] /=2; Set Vx = Vx SHR 1 (Vx is divided by 2.)
                        break;
                    }

                    case 0x7:{
                        ch->V[0xF]=ch->V[y] >= ch->V[x];
                        ch->V[x]= ch->V[y] - ch->V[x];
                        break;
                    }

                    case 0xE:{
                        ch->V[0xF] = (ch->V[x]>>7 ) & 0x1;//V[x] was never bit-masked to 4 bits—only x was.
                        ch->V[x] <<=1;;
                        break;
                    }
                        
                    
                }
                break;
            }

            //case  
            

            default:
                printf("Instruction %#x\n cannot be executed", opcode); //"%#x\n", opcodde); // Outputs: 0xa...  
                break;
        };

        t=0;
        //pc+=2;//calculation done in bytes so 2 byte = 16 bits
    }

    fclose(opcode_file);

}
//  fread(buffer, sizeof(int),5, file);
