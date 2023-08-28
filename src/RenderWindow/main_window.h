//
// Created by eafc6 on 8/27/2023.
//

#ifndef EMULATOR_MAIN_WINDOW_H
#define EMULATOR_MAIN_WINDOW_H

#include <SDL_video.h>
#include <SDL_render.h>

#define VIDEO_WIDTH 256
#define VIDEO_HEIGHT 240

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture *pixels;

void init_video();


#endif //EMULATOR_MAIN_WINDOW_H
