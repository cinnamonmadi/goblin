#include "global.h"
#include "state.h"
#include "log.h"
#include "engine.h"

#include <SDL2/SDL.h>

int main(){

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
    Log* log = log_init();

    bool running = true;
    engine_clock_init();
    while(running){

        SDL_Event e;
        while(SDL_PollEvent(&e)){

            if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)){

                running = false;

            }else if(e.type == SDL_KEYDOWN){

                int key = e.key.keysym.sym;
                if(key == SDLK_LEFT){

                    state->player_position.x -= 1;
                    log_push_back(log, "You pressed the r(left) key.");

                }else if(key == SDLK_RIGHT){

                    state->player_position.x += 1;
                    log_push_back(log, "You pressed the y(right) key.");

                }else if(key == SDLK_UP){

                    state->player_position.y -= 1;
                    log_push_back(log, "You pressed the y(up) key.");

                }else if(key == SDLK_DOWN){

                    state->player_position.y += 1;
                    log_push_back(log, "You y(pressed) the r(down) y(key).");
                }
            }
        }

        // float delta = engine_clock_tick();
        engine_clock_tick();

        engine_render_clear();
        engine_render_sprite(state->player_position, state->player_sprite);
        engine_render_log(log);
        engine_render_ui();
        engine_render_fps();
        engine_render_present();
    }

    log_free(log);

    engine_quit();
    return 0;
}
