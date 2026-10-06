#include <stdio.h>
#include <SDL2/SDL.h>//Angle Brackets <...>: Tells the compiler to look inside the system's global library directory. On Ubuntu, this primary directory is /usr/include/
#include <string.h>
#include "core_loop.h"



//keyboard

//Font: memory (0x000-0x1FF), it is common to store font data there.
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
// struct 
chip8 ch;

// startup function
void start(chip8 *ch){
    //memset() is used to fill a block of memory with a particular value.
    memset(ch->ram,0, sizeof(ch->ram));//ram
    memset(ch->stack, 0, sizeof(ch->stack));//reg zero
    ch->sp=0;//stack pointer
    ch->pc=0x200;
}
void font_set(chip8 *ch){
    memcpy(&ch->ram[0x000], font, sizeof(font));//(&ch->ram[0x000]: memcpy need the address of the first slot not the value in 0x000 value is 0
}

int main(int argc, char *argv[]){

    start(&ch);
    font_set(&ch);

    if(argc < 2){
        printf("No ROM given.\n");
        printf("Use: ./chip8 <program_location>\n");
    }
    
    file_op(argv[1]);//argv[1] is already an address pointing to the first letter of the string
    
    //Initialize SDL display
    printf("Initializing SDL\n");

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        //fprintf( DESTINATION, "MESSAGE WITH PLACEHOLDERS", VALUES );
        fprintf(stderr,"Could not initialize SDL: %s \n", SDL_GetError());
        exit(1); //any non-zero number passed to exit() is considered a failure
    }
    printf("SDL Initialized\n");

    atexit(SDL_Quit);
 
    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP, &sdlWindow, &sdlRenderer);

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderPresent(sdlRenderer);
    // SDL_Delay(5000);

    // if(screen==NULL){
    //     fprintf(stderr, "Could set video mode: %s\n", SDL_GetError());
    //     exit(1);
    // }
 

    printf("Quit SDL\n");
    SDL_Quit();
    printf("Qutting...\n");

    exit(0);
}

