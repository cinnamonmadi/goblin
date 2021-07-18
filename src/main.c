#include "global.h"
#include "state.h"
#include "engine.h"
#include "map.h"

#if __EMSCRIPTEN__
    #include <emscripten/emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

State* state = NULL;
bool running = true;

void loop();

int main(){

    srand(time(NULL));

    bool success = engine_init("Goblin Quest");
    if(!success){

        return 0;
    }

    state = state_init();
    engine_clock_init();

#if __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while(running){

        loop();
    }
#endif

    engine_quit();

    return 0;
}

void loop(){

    SDL_Event e;
    Action action = ACTION_NONE;
    while(SDL_PollEvent(&e)){

        if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)){

            running = false;

        }else if(e.type == SDL_KEYDOWN){

            int key = e.key.keysym.sym;
            if(key == SDLK_LEFT){

                action = ACTION_MOVE_LEFT;

            }else if(key == SDLK_RIGHT){

                action = ACTION_MOVE_RIGHT;

            }else if(key == SDLK_UP){

                action = ACTION_MOVE_UP;

            }else if(key == SDLK_DOWN){

                action = ACTION_MOVE_DOWN;
            }
        }
    }

    // float delta = engine_clock_tick();
    engine_clock_tick();

    state_update(state, action);

    engine_render_clear();

    engine_render_state(state);
    engine_render_ui();

    engine_render_fps();

    engine_render_present();
}
