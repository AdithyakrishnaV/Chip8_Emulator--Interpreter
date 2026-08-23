#include <stdio.h>
#include <SDL2/SDL.h>//Angle Brackets <...>: Tells the compiler to look inside the system's global library directory. On Ubuntu, this primary directory is /usr/include/

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
//declare globally
chip8 *ch;

//keyboard

//Font: memory (0x000-0x1FF), it is common to store font data there.
const uint8_t font[]={
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
// startup function
void start(chip8 *ch){
    //memset() is used to fill a block of memory with a particular value.
    memset(ch->ram,0, sizeof(ch->ram));//ram
    memset(ch->stack, 0, sizeof(ch->stack));//reg zero
    ch->sp=0;//stack pointer
    ch->pc=0x200;
}

int main(int argc, char *argv[]){

    void start(chip8 *ch);

    
    
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

