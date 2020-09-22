// For enum operators: https://stackoverflow.com/a/12080553

#pragma once

#include "simp_types.hpp"

#include <SDL2/SDL.h>
#include <string>

namespace simp
{

inline void vec4ToRect(Vec4 vec, SDL_Rect& out) {
    out = {static_cast<int>(vec.x), static_cast<int>(vec.y), static_cast<int>(vec.w), static_cast<int>(vec.h)};
}

inline SDL_Rect vec4toRect(Vec4 vec) {
    return {static_cast<int>(vec.x), static_cast<int>(vec.y), static_cast<int>(vec.w), static_cast<int>(vec.h)};
}

Vec2 getRenderSize(SDL_Renderer* renderer) {
    int w{0}, h{0};
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return { static_cast<float>(w), static_cast<float>(h) };
}

float getAlignX(SDL_Renderer* renderer, Align align, float x, float w) {
    const auto render_size = getRenderSize(renderer);
    return [&]() {
        switch (align) {
            case Align::LEFT: return x;
            case Align::CENTER: return ((render_size.x / 2) - (w / 2)) + x;
            case Align::RIGHT: return (render_size.x - w);
            default: __builtin_unreachable(); 
        }
    }();
}

TextureLocked lock(SDL_Texture* texture, const SDL_Rect* rect = nullptr) {
    void *pixels{nullptr};
    int pitch{0};
    auto ret = SDL_LockTexture(texture, rect, &pixels, &pitch);
    return TextureLocked{pixels, pitch, ret == 0};
}
TextureLocked lock(SDL_Texture* texture, Vec4 clip) {
    const SDL_Rect rect = {static_cast<int>(clip.x), static_cast<int>(clip.y), static_cast<int>(clip.w), static_cast<int>(clip.h)};
    return lock(texture, &rect);
}

void unlock(SDL_Texture* texture) {
    SDL_UnlockTexture(texture);
}

TextureFormat getTextureFormat(SDL_Texture* texture) {
    u32 format;
    int access, w, h;
    SDL_QueryTexture(texture, &format, &access, &w, &h);
    return TextureFormat{format, access, w, h};
}

Colour getRendererColour(SDL_Renderer* renderer) {
    u8 r,g,b,a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    return Colour{r,g,b,a};
}
int setRenderColour(SDL_Renderer* renderer, const Colour col) {
    return SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
}

int renderClear(SDL_Renderer* renderer, Colour colour) {
    setRenderColour(renderer, colour);
    return SDL_RenderClear(renderer);
}

int renderCopy(SDL_Renderer* renderer, SDL_Texture* texture) {
    return SDL_RenderCopy(renderer, texture, nullptr, nullptr);
}
int renderCopy(SDL_Renderer* renderer, SDL_Texture* texture, Vec4 src, Vec4 dst) {
    const auto src_rect = vec4toRect(src);
    const auto dst_rect = vec4toRect(dst);
    return SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
}

int setRenderBlendMode(SDL_Renderer* renderer, const BlendMode mode) {
    return SDL_SetRenderDrawBlendMode(renderer, static_cast<SDL_BlendMode>(mode));
}
BlendMode getRenderBlendMode(SDL_Renderer* renderer) {
    SDL_BlendMode mode;
    SDL_GetRenderDrawBlendMode(renderer, &mode);
    return static_cast<BlendMode>(mode);
}

int setRenderScale(SDL_Renderer* renderer, const Vec2 vec) {
    return SDL_RenderSetScale(renderer, vec.x, vec.y);
}
Vec2 getRenderScale(SDL_Renderer* renderer) {
    float x,y;
    SDL_RenderGetScale(renderer, &x, &y);
    return {x,y};
}

int setTextureBlendMode(SDL_Texture* texture, BlendMode mode) {
    return SDL_SetTextureBlendMode(texture, static_cast<SDL_BlendMode>(mode));
}
BlendMode getTextureBlendMode(SDL_Texture* texture) {
    SDL_BlendMode mode;
    SDL_GetTextureBlendMode(texture, &mode);
    return static_cast<BlendMode>(mode);
}

int setTextureScaleMode(SDL_Texture* texture, ScaleMode mode) {
    return SDL_SetTextureScaleMode(texture, static_cast<SDL_ScaleMode>(mode));
}
ScaleMode getTextureScaleMode(SDL_Texture* texture) {
    SDL_ScaleMode mode;
    SDL_GetTextureScaleMode(texture, &mode);
    return static_cast<ScaleMode>(mode);
}

void setWindowTitle(SDL_Window* window, const std::string& title) {
    SDL_SetWindowTitle(window, title.c_str());
}

void setWindowIcon(SDL_Window* window, SDL_Surface* surface, bool free = false) {
    SDL_SetWindowIcon(window, surface);
    if (free) {
        SDL_FreeSurface(surface);
    }
}

SDL_Window* createWindow(const char* title, int x, int y, int w, int h, WindowFlags flags) {
    return SDL_CreateWindow(title, x, y, w, h, static_cast<u32>(flags));
}
SDL_Window* createWindow(const std::string& title, int x, int y, int w, int h, WindowFlags flags) {
    return createWindow(title.c_str(), x, y, w, h, flags);
}
SDL_Window* createWindow(const char* title, int w, int h, WindowFlags flags) {
    return createWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
}
SDL_Window* createWindow(const std::string& title, int w, int h, WindowFlags flags) {
    return createWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
}

SDL_Renderer* createRenderer(SDL_Window* window, RenderFlags flags, int index = -1, BlendMode blend_mode = BlendMode::NONE) {
    auto renderer = SDL_CreateRenderer(window, index, static_cast<u32>(flags));
    if (renderer) {
        setRenderBlendMode(renderer, blend_mode);
    }
    return renderer;
}

SDL_Texture* createTexture(SDL_Renderer* renderer, int w, int h, PixelFormat format = PixelFormat::ARGB8888, TextureAccess access = TextureAccess::STREAMING, BlendMode blend_mode = BlendMode::NONE, ScaleMode scale_mode = ScaleMode::NEAREST) {
    auto texture = SDL_CreateTexture(renderer, static_cast<u32>(format), static_cast<int>(access), w, h);
    if (texture) {
        setTextureBlendMode(texture, blend_mode);
        setTextureScaleMode(texture, scale_mode);
    }
    return texture;
}

inline void sleep(u32 ms) { SDL_Delay(ms); }

} // namespace simp
