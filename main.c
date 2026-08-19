#include <stdio.h>
#include <SDL2/SDL.h>//Angle Brackets <...>: Tells the compiler to look inside the system's global library directory. On Ubuntu, this primary directory is /usr/include/

struct chip8{
    uint8_t ram[4096]; //8bits=1byte, 1byte x 4096bytes = 4kb
    uint16_t pc;
    uint16_t I;
    uint16_t stack[16];
    uint8_t sp; //stack pointer
    uint8_t dTimer;
    uint8_t sTimer;
    uint8_t V[16];
}ch;

//keyboard


int main(int argc, char *argv[]){

    
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

