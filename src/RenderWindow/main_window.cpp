//
// Created by eafc6 on 8/27/2023.
//

#include <SDL.h>
#include "main_window.h"
#include "spdlog/spdlog.h"
#include "../global.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;


void render_frame(RGB frame_buffer[256][240]) {
    void* pixels;
    int pitch;

    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
        spdlog::critical("SDL_LockTexture failed: {}\n", SDL_GetError());
        return;
    }

    uint32_t* texture_pixels = static_cast<uint32_t*>(pixels);
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 240; x++) {
            RGB color = frame_buffer[y][x];
            uint32_t packed_color = (color.r << 16) | (color.g << 8) | color.b;
            texture_pixels[y * (pitch / 4) + x] = packed_color;
        }
    }

    SDL_UnlockTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}


void init_video(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::critical("Could not initialize SDL: {}\n", SDL_GetError());
        exit();
    }
    window = SDL_CreateWindow("NESEmu",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,
                                 SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STREAMING,
                                 VIDEO_WIDTH, VIDEO_HEIGHT);
}