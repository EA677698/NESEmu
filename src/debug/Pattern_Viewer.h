//
// Created by eafc6 on 7/28/2025.
//

#ifndef PATTERN_VIEWER_H
#define PATTERN_VIEWER_H
#include "../PPU/ppu.h"
#include <SDL_video.h>
#include <SDL_render.h>

#define PATTERN_WINDOW_WIDTH 260
#define PATTERN_WINDOW_HEIGHT 128


class Pattern_Viewer {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

public:

    Pattern_Viewer();

    void render(PPU* ppu);

    ~Pattern_Viewer();

};



#endif //PATTERN_VIEWER_H
