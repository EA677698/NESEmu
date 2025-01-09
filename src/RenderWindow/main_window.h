//
// Created by eafc6 on 8/27/2023.
//

#ifndef EMULATOR_MAIN_WINDOW_H
#define EMULATOR_MAIN_WINDOW_H

#include <SDL_video.h>
#include <SDL_render.h>
#include "../global.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture *texture;

void init_video();

void render_frame(RGBA frame_buffer[VIDEO_HEIGHT][VIDEO_WIDTH]);


#endif //EMULATOR_MAIN_WINDOW_H
