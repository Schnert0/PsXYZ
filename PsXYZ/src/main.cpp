#include <SDL.h>
#include <GL/glfw.h>
#include <stdio.h>
#include <string.h>

#include "system/system.h"

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("PsXYZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 512, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
    SDL_GL_CreateContext(window);
    glfwInit();
    glOrtho(0, 1024, 512, 0, -1, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    c_system system;

    system.loadBIOS("BIOS/SCPH1001.bin");

    system.reset();
    bool running = true;
    while(running){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch(event.window.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                    case SDLK_F1:
                        system.reset();
                        break;
                    }
                    break;
            }
        }
        system.run();
        SDL_GL_SwapWindow(window);
    }

    return 0;
}
