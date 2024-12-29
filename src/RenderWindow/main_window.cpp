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


void render_frame(RGBA frame_buffer[VIDEO_HEIGHT][VIDEO_WIDTH]) {

    if (SDL_UpdateTexture(texture, nullptr, frame_buffer, VIDEO_WIDTH * sizeof(RGBA)) != 0) {
        spdlog::critical("SDL_UpdateTexture failed: {}\n", SDL_GetError());
        return;
    }

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