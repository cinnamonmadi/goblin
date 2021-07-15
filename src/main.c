#include "global.h"
#include "state.h"
#include "log.h"
#include "engine.h"
#include "map.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int main(){

    srand(time(NULL));

    bool success = engine_init("Goblin Quest");
    if(!success){

        return 0;
    }

    int chatlog_length = 10;
    char** chatlog = malloc(sizeof(char*) * chatlog_length);
    for(int i = 0; i < chatlog_length; i++){
        chatlog[i] = malloc(sizeof(char) * 102);
        for(int j = 0; j < 102; j++){
            chatlog[i][j] = 'a' + (j % 26);
        }
    }

    State* state = state_init();
    state_update_sidebar_info(state);
    Log* log = log_init();

    bool running = true;
    engine_clock_init();
    while(running){

        SDL_Event e;
        int action = ACTION_NONE;
        while(SDL_PollEvent(&e)){

            if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)){

                running = false;

            }else if(e.type == SDL_KEYDOWN){

                int key = e.key.keysym.sym;
                if(key == SDLK_LEFT){

                    action = ACTION_MOVE_LEFT;
                    log_push_back(log, "You pressed the r(left) key.");

                }else if(key == SDLK_RIGHT){

                    action = ACTION_MOVE_RIGHT;
                    log_push_back(log, "You pressed the y(right) key.");

                }else if(key == SDLK_UP){

                    action = ACTION_MOVE_UP;
                    log_push_back(log, "You pressed the y(up) key.");

                }else if(key == SDLK_DOWN){

                    action = ACTION_MOVE_DOWN;
                    log_push_back(log, "You y(pressed) the r(down) y(key).");
                }
            }
        }

        // float delta = engine_clock_tick();
        engine_clock_tick();

        state_update(state, action);

        engine_render_clear();

        engine_render_state(state);
        engine_render_log(log);
        engine_render_sidebar(state->sidebar_info, SIDEBAR_INFO_LENGTH);
        engine_render_ui();

        engine_render_fps();

        engine_render_present();
    }

    log_free(log);

    engine_quit();
    return 0;
}
