//
// Created by eafc6 on 5/25/2025.
//

#include "s_palette_viewer.h"

SPalette_Viewer::SPalette_Viewer() {
    window = SDL_CreateWindow("System Palette Viewer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              S_PALETTE_WINDOW_WIDTH, S_PALETTE_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                S_PALETTE_WINDOW_WIDTH, S_PALETTE_WINDOW_HEIGHT);
}

void SPalette_Viewer::render(PPU *ppu) {
    SDL_RenderClear(renderer);
    for (int i = 0; i < 0x3F; i++) {
        RGBA color = ppu->get_rgb_from_palette(i);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

        int x = (i % 16) * 16;
        int y = (i / 16) * 16;
        SDL_Rect rect = {x, y, 16, 16};
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

SPalette_Viewer::~SPalette_Viewer() {
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
