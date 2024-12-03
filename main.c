#include <stdio.h>
#include <SDL2/SDL.h>

// for initializing and shutdown functions
#include <SDL2/SDL.h>
// for rendering images and graphics on screen
//#include <SDL2/SDL_image.h>

// for using SDL_Delay() functions
#include <SDL2/SDL_timer.h>


struct chip8structure{ 
    //memory 8bit*4096
	uint8_t memory[4096];
	//PC -> program counter 2 bytes
	uint16_t PC; // 0x200
	// I -> index register 2 bytes
	uint16_t I;
	// 8 bit delay timer
	uint8_t Timer;
	// 8 bit sound timer
	uint8_t sound;
	// Stack 16 bit 16 level-> structure
	uint16_t stack[16];
	//stack pointer
	uint8_t sp;
	//16 : 8bit general purpose register
	uint8_t Register[16];
	//display
	uint8_t display[64*32];
};
//declare pointer globally
struct chip8structure *c8; 

//font 16 characters × 5 bytes=80 bytes.
const uint8_t font[80]={
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
void fontspace(struct chip8structure *c8){
	 memcpy(c8->memory, font, sizeof(font));
};
//void *memcpy(void *to, const void *from, size_t numBytes);

//********** Clear the memory, registers and screen ************
int Initialize(struct chip8structure *c8){
	//clean memory
	memset(c8->memory, 0, sizeof(c8->memory));
	//clean register
	memset(c8->Register,0,sizeof(c8->Register));
	
	c8->PC=0;
	c8->I=0;
	c8->sound=0;
	c8->Timer=0;

	memset(c8->stack,0,sizeof(c8->stack));
	c8->sp=0;

	//clear screen 00E0
	memset(c8->display,0,sizeof(c8->display));

	return 0;
}

//*************** Copy the program into the memory ***************
void chip8(const char * filename ){// In C, when variable is passed as a parameter its type need to be definespecifiedd
		FILE * file = fopen(filename,"rb"); //read binary mode
		
		if(file==NULL){
			printf("Error, file not opened\n");
			exit(-1);
		}else{
			printf("file opened %s \n", filename);
		}
		//starting address //0x200
		
		uint16_t readByte= fread(c8->memory+0x200, 1, sizeof(c8->memory)-0x200, file);

		//-----------Print the memory contents after reading
		for(int i=0x200; i<0x200+readByte; i+=2){
			printf("Memory[%03X] = %02X\n",i, c8->memory[i]);
		}

		c8->PC=0x200; //PC holds address

		//Emulate one cycle of the system
		int running = 1;//true
		while(running){
			//fetch
			uint16_t opcode= c8->memory[c8->PC] << 8 | c8->memory[c8->PC+1];//fetch instruction

			//------------Print to debug error in reading
			printf("PC = %03X, Memory[PC] = %02X, Memory[PC+1] = %02X\n", c8->PC, c8->memory[c8->PC], c8->memory[c8->PC + 1]);
			printf("opcode= %04x\n",opcode);

			//decode //execute
			//*********handle the opcodes.*************
			uint8_t type = (opcode&0xF000) >> 12;//right shift 12 bits -> 0110 0000 0000 0000 >> 12 = 0000 0000 0000 0110
			uint16_t NNN = opcode&0x0FFF;
			uint8_t  NN = opcode&0x00FF;
			uint8_t  N = opcode&0x000F;
			uint16_t X = opcode&0x0F00;
			uint16_t Y= opcode&0x00F0;
			
			switch(type){//DEcode first nibble
				case 0x0:
				{
					switch(NNN){// handle special case for 0x0NNN
						case 0x00E0:{
						//clear screen
						memset(c8->display,0,sizeof(c8->display));
						printf("screen clear\n");
						break; 
						}
						
						case 0x0EE:{
							c8->stack[c8->sp]=c8->PC; //sets program counter to the address at the top of the stack
							c8->sp--;//subtracts 1 from the stack pointer
							break;
						}

						default:
							break;
					}

					break; 
				}
				
				case 0x1:{
					c8->PC=NNN;//sets the PC to nnn.

					break;
				}
				
				case 0x2:{//Call subroutine at nnn.
					c8->sp++;//increments the stack pointer
					c8->stack[c8->sp]= c8->PC;//puts the current PC on the top of the stack
					c8->PC=NNN;//PC is set to nnn
					break;
				}

				case 0x3:{
					
					break;
				}


				default:
				printf("unknown opcode: 0x%04X\n", opcode);
				running = 0;
			}
			
			
			c8->PC += 2;//add 2bytes from 0x002 which is the nxt opcode
		 	
			if(c8->PC > 0xFFF){ // CHIP-8 memory is 4,096 bytes (from 0x0000 to 0xFFF)
				running =0;
			}
		}
		// close file
		fclose(file);
		
}

int main(int argc,char *argv){
	const char * filename = "IBM_Logo.ch8";
	// Memory allocated on the heap
	c8 = malloc(sizeof(struct chip8structure)); //allocate at runtime, malloc is a run time function
	if(c8 == NULL){
		printf("Cannot allocate memory\n");
		exit(1);//exit the program if memory allocation failed
	}else{
		printf("Memory allocated successfully\n");
	}
	
	Initialize(c8);
	chip8(filename);

	//initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)==-1){
		fprintf(stderr,"SDL not initialized,%s\n", SDL_GetError());
		return -1;// Return an error code
	}
	
	printf("SDL initialized\n");
	
	//Initializing display 64x32
	
        SDL_Window * window = SDL_CreateWindow( "chip8",
                              SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, 640,
                               320, 0x00000004);
	
	SDL_Delay(5000);
	if(window ==NULL){
		fprintf(stderr,"Couldn't initialize display, %s\n",SDL_GetError());
		exit(1);
	}
	
	SDL_Quit();
	printf("Quitting.....\n");

	free(c8);// free the allocated heap memory of chip8structure
	
	return 0;
 }
