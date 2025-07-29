//
// Created by eafc6 on 5/25/2025.
//

#ifndef PALETTE_VIEWER_H
#define PALETTE_VIEWER_H

#include "../PPU/ppu.h"
#include <SDL_video.h>
#include <SDL_render.h>

#define PALETTE_WINDOW_WIDTH 256
#define PALETTE_WINDOW_HEIGHT 32

class Palette_Viewer {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

public:

    Palette_Viewer();

    void render(PPU* ppu);

    ~Palette_Viewer();

};


#endif //PALETTE_VIEWER_H
