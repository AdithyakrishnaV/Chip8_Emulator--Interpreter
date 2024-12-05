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
	//16 : 8bit general purpose Register
	uint8_t V[16];
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
	memset(c8->V,0,sizeof(c8->V));
	
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
		c8->V[0xF] = 0;//Clear VF flag before starting the loop
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
			uint16_t X = (opcode&0x0F00) >> 8;
			uint16_t Y= (opcode&0x00F0) >> 4;
			printf("X = %02X\n", X);
			printf("Y = %02X\n", Y);
			switch(type){//Decode first nibble
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
					if(c8->V[X]==NN){
						c8->PC+=2;//increments the program counter by 2
					}
					break;
				}

				case 0x4:{
					if(c8->V[X] != NN){
						c8->PC+=2;
					}
					break;
				}

				case 0x5:{
					if(c8->V[X]==c8->V[Y]){
						c8->PC+=2;
					}
					break;
				}

				case 0x6:{
					c8->V[X]==NN;
					break;
				}

				case 0x7:{
					c8->V[X]+=NN;
					break;
				}

				case 0x8:{
					switch(N){// Extract the last nibble

						case 0x0:{
						c8->V[X]=c8->V[Y];
						break;
						}

						case 0x1:{
							c8->V[X] |= c8->V[Y];
							break;
						}

						case 0x2:{
							c8->V[X] &= c8->V[Y]; 
							break;
						}

						case 0x3:{
							c8->V[X] ^= c8->V[Y]; 
							break;
						}

						case 0x4:{
							uint16_t result = c8->V[X] + c8->V[Y];//16-bit variable (result) used to preserve the full result or it will be truncated to 8bit
							c8->V[0xF] =((result)>0xFF) ? 1:0;// If the result exceeds 0xFF (255 in decimal), it means there was an overflow, and the carry flag (VF) is set to 1. Otherwise, 0.
							c8->V[X]=result & 0xFF;//Store only the lowest 8 bits.
							break;
						}

						case 0x5:{
							c8->V[0xF]=(c8->V[X]>=c8->V[Y])?1:0;
							c8->V[X]-=c8->V[Y];
							 break;
						}

						case 0x6:{//0x1 in binary is 00000001
							c8->V[0xF] = c8->V[X]&0X1;//bitwise AND operation with V[X], isolate the least significant bit (LSB) of V[X].
							c8->V[X]>>=1;
							break;
						}

						case 0x7:{
							c8->V[0XF] = (c8->V[Y]>-c8->V[X])?1:0;
							c8->V[X] = c8->V[Y] - c8->V[X];
							break;
						}	

						case 0xE:{
							c8->V[0xF]=(c8->V[X] & 0x80)>>7;
							c8->V[X]<<=1;
							break;
						}

						default:
							break;
					}

					break;
				}

				case 0x9:{
					if(c8->V[X] != c8->V[Y]){
						c8->PC+=2;
					}
					break;
				}

				case 0xA:{
					c8->I=NNN;
					break;
				}

				case 0xB:{
					c8->PC=NNN+c8->V[0];
					break;
				}

				case 0xC:{
					c8->V[X]= (rand()%256) & NN;//% 256 ensures that the result is within the range 0–255
					break;
				}

				case 0xD: { 
					uint8_t x = c8->V[X]; // X position
					uint8_t y = c8->V[Y]; // Y position
					uint8_t n = N;         // Height of sprite (in bytes)
					uint8_t spriteByte;    // To hold each byte of the sprite

					c8->V[0xF] = 0; // Clear VF flag before drawing

					for (int row = 0; row < n; row++) {
						// Fetch the sprite byte from memory
						spriteByte = c8->memory[c8->I + row];
						
						for (int col = 0; col < 8; col++) {
							// Get the current pixel (bitwise operation)
							uint8_t pixel = (spriteByte >> (7 - col)) & 0x1;
							int screenX = (x + col) % 64; // Wrap around horizontally
							int screenY = (y + row) % 32; // Wrap around vertically

							// Check if the pixel is on the screen
							if (pixel) {
								// If the pixel is on, XOR it with the existing screen pixel
								if (c8->display[screenY * 64 + screenX] == 1) {
									c8->V[0xF] = 1; // Set VF flag if pixel is erased (collision)
								}
								c8->display[screenY * 64 + screenX] ^= 1; // XOR operation
							}
						}
					}
					break;
				}

				case 0xE:{
					switch (NN)
					{
					case 0x9E:{
						break;
					}
					case 0xA1:{
						break;
					}	
						
					
					default:
						break;
					}
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
	if(SDL_Init(SDL_INIT_VIDEO)!=0){//SDL_Init function returns 0 on success and a negative error code on failure
		fprintf(stderr,"SDL not initialized,%s\n", SDL_GetError());
		SDL_Quit();
		return -1;// Return an error code
	}
	
	printf("SDL initialized\n");
	
	//Initializing display 64x32
	
        SDL_Window * window = SDL_CreateWindow( "chip8",
                              SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, 840,
                               520, 0x00000004);//  SDL_WINDOW_SHOWN = 0x00000004
	
	//error handling
	if(window ==NULL){
		fprintf(stderr,"Couldn't initialize display:, %s\n",SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	// Create a renderer
	SDL_Renderer *renderer= SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

	//error handling
	if(!renderer){
		fprintf(stderr,"Couldn't create renderer:, %s\n",SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	//create texture
	SDL_Texture *texture= SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64,32);

	//error handling
	if(!texture){
		fprintf(stderr,"Couldn't create texture:, %s\n",SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	//event loop
	SDL_Event event;
	int run=1;
	while (run)
	{
		while(SDL_PollEvent(&event)){
			//check for events
			if(event.type == SDL_QUIT){
				run=0;
			}
		}
//64 * 32 = 2048 elements. Initializes all elements of the pixels array to 0 or "off"
		uint32_t pixels[64 * 32]={0};//pixels array stores color of each pixel in chip8 display
		for(int i=0; i<32; i++){//row
			for(int j=0; j<64; j++){//column
			//retrive value of the pixel in the chip8 display array
				uint8_t pixel = c8->display[i * 64 + j]; 
				pixels[i * 64 + j] = pixel? 0xFFFFFFFF : 0xFF000000;
				//0xFFFFFFFF = white | if pixel non-zero then white
			}
		}

		SDL_UpdateTexture(texture,NULL, pixels, 64 * sizeof(uint32_t));// pitch : number of bytes in a row of pixel 

		//Render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,texture,NULL,NULL);
		SDL_RenderPresent(renderer);
	}




	//clean-up
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	printf("Quitting.....\n");
    
	free(c8);// free the allocated heap memory of chip8structure
	
	return 0;
 }
