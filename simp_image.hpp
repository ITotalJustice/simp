#pragma once

#include "simp_fs.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb/stb_image.h"

#include <SDL2/SDL.h>
#include <string>
#include <vector>

namespace simp {

SDL_Surface* loadImage(const u8* data, int size) {
    int w{0}, h{0}, comp{0};

    const auto info = stbi_info_from_memory(data, size, &w, &h, &comp);
    if (info == STBI_default) {
        return nullptr;
    }

    // grey is okay.
    const auto wanted_format = (info == STBI_grey || info == STBI_rgb) ? STBI_rgb : STBI_rgb_alpha;

    auto loaded_image = stbi_load_from_memory(data, size, &w, &h, &comp, wanted_format);
    if (loaded_image == nullptr) {
        return nullptr;
    }

    // https://wiki.libsdl.org/SDL_CreateRGBSurface
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const u32 rmask = 0xff000000;
        const u32 gmask = 0x00ff0000;
        const u32 bmask = 0x0000ff00;
        const u32 amask = 0x000000ff;
    #else
        const u32 rmask = 0x000000ff;
        const u32 gmask = 0x0000ff00;
        const u32 bmask = 0x00ff0000;
        const u32 amask = 0xff000000;
    #endif

    // we have to create sdl surface this way as the other methods do not
    // create its own copy of the pixel data that we pass it.
    // The other methods causes issues because we have to track when we free
    // the surface and then *after* free the stbi_image.
    auto surface = SDL_CreateRGBSurface(0, w, h, wanted_format * 8, rmask, gmask, bmask, amask);

    // copy the pixels, pray we don't segafult.
    memcpy(surface->pixels, static_cast<void*>(loaded_image), w * h * wanted_format);
    
    // we didn't segafult, nice.
    stbi_image_free(loaded_image);

    return surface;
}
SDL_Surface* loadImage(const char* path) {
    Fs fs{path, std::ios_base::binary};
    if (!fs.is_open()) {
        return nullptr;
    }

    const auto size = fs.size();
    std::vector<u8> data(size);
    fs.read(reinterpret_cast<char*>(data.data()), data.size());
    if (!fs.good()) {
        return nullptr;
    }

    return loadImage(data.data(), static_cast<int>(data.size()));    
}
SDL_Surface* loadImage(const std::string& path) {
    return loadImage(path.c_str());
}
SDL_Texture* loadImage(SDL_Renderer* renderer, const char* path) {
    auto surface = loadImage(path);
    if (surface == nullptr) {
        return nullptr;
    }
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
SDL_Texture* loadImage(SDL_Renderer* renderer, const std::string& path) {
    return loadImage(renderer, path.c_str());
}

} // namespace
