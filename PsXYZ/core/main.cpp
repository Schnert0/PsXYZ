#include <SDL.h>
#include <GL/glfw3.h>
#include <stdio.h>
#include <string.h>

#include "../renderer/renderer.h"
#include "system/system.h"

int main(int argc, char* argv[]){
  SDL_Init(SDL_INIT_EVERYTHING);

  Renderer renderer;
  System system(&renderer);

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
    renderer.update();
  }

  return 0;
}
