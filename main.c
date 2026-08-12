#include <stdio.h>
#include <SDL2/SDL.h>//Angle Brackets <...>: Tells the compiler to look inside the system's global library directory. On Ubuntu, this primary directory is /usr/include/

int main(int argc, char *argv[]){
    //Initialize SDL display
    printf("Initializeing SDL\n");
    
    if ((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)){
        printf("Could not initialize SDL: %s\n",SDL_GetError());
        exit(-1);
        //If you remove exit(-1), your program will not stop. It will keep running and immediately crash with a Segfault (Segmentation Fault) 
    }
    printf("SDL Initialized\n");
    printf("Quit SDL\n");

    SDL_Quit();
    printf("Qutting...\n");

    exit(0);
}

