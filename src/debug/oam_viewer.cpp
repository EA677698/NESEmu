//
// Created by eafc6 on 5/25/2025.
//

#include "oam_viewer.h"

OAM_Viewer::OAM_Viewer() {
    window = SDL_CreateWindow("OAM Viewer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              OAM_WINDOW_WIDTH, OAM_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                OAM_WINDOW_WIDTH, OAM_WINDOW_HEIGHT);
}

void OAM_Viewer::render(PPU* ppu) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    uint8_t* OAM = ppu->get_OAM();
    for (int i = 0; i < 64; i++) {
        int base = i * 4;
        uint8_t y = OAM[base];
        uint8_t tile_index = OAM[base + 1];
        uint8_t attr = OAM[base + 2];
        uint8_t x = OAM[base + 3];

        SDL_Rect rect = {
                (i % 16) * 16,
                (i / 16) * 16,
                16, 16
        };

        uint8_t r = (attr & 0x3) * 85;
        SDL_SetRenderDrawColor(renderer, r, 255 - r, 128, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

OAM_Viewer::~OAM_Viewer() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
}
