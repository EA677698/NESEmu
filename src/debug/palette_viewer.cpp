//
// Created by eafc6 on 5/25/2025.
//

#include "palette_viewer.h"

Palette_Viewer::Palette_Viewer() {
    window = SDL_CreateWindow("Palette Viewer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              PALETTE_WINDOW_WIDTH, PALETTE_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                PALETTE_WINDOW_WIDTH, PALETTE_WINDOW_HEIGHT);
}

void Palette_Viewer::render(PPU *ppu) {
    SDL_RenderClear(renderer);
    uint8_t* palette_ram = ppu->get_palette_ram();
    for(int i = 0; i < 32; i++){
        uint8_t color_index = palette_ram[i];
        RGBA color = ppu->get_rgb_from_palette(color_index);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

        int x = (i % 16) * 16;
        int y = (i / 16) * 16;
        SDL_Rect rect = {x, y, 16, 16};
        SDL_RenderFillRect(renderer, &rect);
    }

    // lets check to see if the system palette loaded correctly
//    for (int i = 0; i < 0x3F; i++){
//        RGBA color = ppu->get_rgb_from_palette(i);
//        SDL_SetRenderDrawColor(palette_renderer, color.r, color.g, color.b, 255);
//
//        int x = (i % 16) * 16;
//        int y = (i / 16) * 16;
//        SDL_Rect rect = {x, y, 16, 16};
//        SDL_RenderFillRect(palette_renderer, &rect);
//    }

    SDL_RenderPresent(renderer);
}

Palette_Viewer::~Palette_Viewer() {
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
