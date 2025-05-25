//
// Created by eafc6 on 5/25/2025.
//

#ifndef OAM_WINDOW_H
#define OAM_WINDOW_H

#include "spdlog/spdlog.h"
#include "../PPU/ppu.h"
#include <SDL_video.h>
#include <SDL_render.h>

#define OAM_WINDOW_WIDTH 256
#define OAM_WINDOW_HEIGHT 128

class OAM_Viewer {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

public:

    OAM_Viewer();

    void render(PPU* ppu);

    ~OAM_Viewer();


};


#endif //OAM_WINDOW_H
