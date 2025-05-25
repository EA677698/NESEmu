//
// Created by eafc6 on 5/25/2025.
//

#ifndef SYSTEM_PALETTE_VIEWER_H
#define SYSTEM_PALETTE_VIEWER_H

#include "../PPU/ppu.h"
#include <SDL_video.h>
#include <SDL_render.h>

#define S_PALETTE_WINDOW_WIDTH 256
#define S_PALETTE_WINDOW_HEIGHT 128


class SPalette_Viewer {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

public:

    SPalette_Viewer();

    void render(PPU* ppu);

    ~SPalette_Viewer();

};


#endif //SYSTEM_PALETTE_VIEWER_H
