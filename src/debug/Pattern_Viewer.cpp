//
// Created by eafc6 on 7/28/2025.
//

#include "Pattern_Viewer.h"

Pattern_Viewer::Pattern_Viewer() {
    window = SDL_CreateWindow("Pattern Viewer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              PATTERN_WINDOW_WIDTH, PATTERN_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                PATTERN_WINDOW_WIDTH, PATTERN_WINDOW_HEIGHT);
}

void Pattern_Viewer::render(PPU *ppu) {
    SDL_RenderClear(renderer);
    uint8_t* table1 = ppu->get_pattern_1_table();
    uint8_t* table2 = ppu->get_pattern_2_table();
    for(int i = 0; i < 128; i++){
        uint8_t color_index = rand() % 0x3F;
        RGBA color = ppu->get_rgb_from_palette(color_index);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

        int x = (i % 16) * 16;
        int y = (i / 16) * 16;
        SDL_Rect rect = {x, y, 16, 16};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

Pattern_Viewer::~Pattern_Viewer() {
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
