#include "engine.h"

#include <SDL2/SDL.h>

int main(){

    bool success = engine_init("Goblin Quest");
    if(!success){

        return 0;
    }

    bool running = true;
    engine_clock_init();
    while(running){

        SDL_Event e;
        while(SDL_PollEvent(&e)){

            if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)){

                running = false;
            }
        }

        // float delta = engine_clock_tick();
        engine_clock_tick();

        engine_render_clear();
        engine_render_fps();
        engine_render_present();
    }

    engine_quit();
    return 0;
}
