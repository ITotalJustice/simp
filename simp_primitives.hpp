#pragma once

#include "simp_types.hpp"
#include "simp_helpers.hpp"

#include <SDL2/SDL.h>

namespace simp {

void drawRect(SDL_Renderer* renderer, const Vec4& vec, const Colour& col, bool filled = true, Align align = Align::LEFT) {
    setRenderColour(renderer, col);

    const SDL_FRect rect{getAlignX(renderer, align, vec.x, vec.w), vec.y, vec.w, vec.h};

    if (filled) {
        SDL_RenderFillRectF(renderer, &rect);
    } else {
        SDL_RenderDrawRectF(renderer, &rect);
    }
}

} // namespace simp
