#pragma once

#include "global.h"
#include "log.h"

#include <stdbool.h>

extern const int SCREEN_WIDTH;
extern const int SCEEN_HEIGHT;

bool engine_init(const char* title);
void engine_quit();

void engine_set_resolution(int width, int height);
void engine_toggle_fullscreen();

void engine_clock_init();
float engine_clock_tick();

void engine_render_clear();
void engine_render_present();

void engine_render_ui();
void engine_render_log(Log* log);

void engine_render_fps();
void engine_render_sprite(const Vector position, const Vector sprite);
