#pragma once

#include <stdbool.h>

extern const int SCREEN_WIDTH;
extern const int SCEEN_HEIGHT;

bool engine_init(char* title);
void engine_quit();

void engine_set_resolution(int width, int height);
void engine_toggle_fullscreen();

void engine_clock_init();
float engine_clock_tick();

void engine_render_clear();
void engine_render_present();

void engine_render_fps();
