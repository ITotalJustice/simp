// Adapted from: https://gist.github.com/benob/92ee64d9ffcaa5d3be95edbf4ded55f2

#pragma once

#include "simp_fs.hpp"
#include "simp_types.hpp"
#include "simp_primitives.hpp"

#ifndef STB_RECT_PACK_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#endif

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#endif

#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"

#include <SDL2/SDL.h>

#include <string>
#include <vector>

namespace simp {

class Font {
    public:
        Font(void) {
            this->info = static_cast<stbtt_fontinfo*>(SDL_malloc(sizeof(stbtt_fontinfo)));
            this->chars = static_cast<stbtt_packedchar*>(SDL_malloc(sizeof(stbtt_packedchar) * 96));
        }
        Font(SDL_Renderer *renderer, const char* path, int _font_size) : Font() {
            this->load(renderer, path, _font_size);
        }
        Font(SDL_Renderer *renderer, const std::string& path, int _font_size) : Font() {
            this->load(renderer, path, _font_size);
        }
        Font(SDL_Renderer *renderer, Fs& fs, int _font_size) : Font() {
            this->load(renderer, fs, _font_size);
        }
        Font(SDL_Renderer *renderer, const u8 *data, u64 data_size, int _font_size) : Font() {
            this->load(renderer, data, data_size, _font_size);
        }

        bool load(SDL_Renderer *renderer, const std::string& path, int _font_size) {
            return this->load(renderer, path.c_str(), _font_size);
        }
        bool load(SDL_Renderer *renderer, const char* path, int _font_size) {
            Fs fs{path, std::ios_base::binary};
            return fs.is_open() && this->load(renderer, fs, _font_size);
        }
        bool load(SDL_Renderer *renderer, Fs& fs, int _font_size) {
            const auto file_size = fs.size();
            std::vector<u8> data(file_size);
            fs.read(reinterpret_cast<char*>(data.data()), data.size());
            return fs.good() && this->load(renderer, data.data(), data.size(), _font_size);
        }
        bool load(SDL_Renderer *renderer, const u8* data, u64 data_size, int _font_size) {
            this->renderer = renderer;

            if (this->atlas) {
                SDL_DestroyTexture(this->atlas);
            }

            if (stbtt_InitFont(this->info, data, 0) == 0) {
                return false;
            }

            // fill bitmap atlas with packed characters
            this->texture_size = 32;
            std::vector<u8> bitmap(this->texture_size * this->texture_size);

            while (1) {
                stbtt_pack_context pack_context;
                stbtt_PackBegin(&pack_context, bitmap.data(), this->texture_size, this->texture_size, 0, 1, 0);
                stbtt_PackSetOversampling(&pack_context, 1, 1);
                if (!stbtt_PackFontRange(&pack_context, data, 0, _font_size, 32, 95, this->chars)) {
                    // too small
                    stbtt_PackEnd(&pack_context);
                    this->texture_size *= 2;
                    bitmap.resize(this->texture_size * this->texture_size);
                } else {
                    stbtt_PackEnd(&pack_context);
                    break;
                }
            }

            // convert bitmap to texture
            this->atlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, this->texture_size, this->texture_size);
            SDL_SetTextureBlendMode(this->atlas, SDL_BLENDMODE_BLEND);

            std::vector<u32> pixels(this->texture_size * this->texture_size);
            SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
            for (auto i = 0; i < this->texture_size * this->texture_size; i++) {
                pixels[i] = SDL_MapRGBA(format, 0xff, 0xff, 0xff, bitmap[i]);
            }
            SDL_UpdateTexture(this->atlas, NULL, pixels.data(), this->texture_size * sizeof(u32));
            SDL_FreeFormat(format);

            // setup additional info
            this->scale = stbtt_ScaleForPixelHeight(this->info, _font_size);
            stbtt_GetFontVMetrics(this->info, &this->ascent, 0, 0);
            this->baseline = static_cast<int>(this->ascent * this->scale);
            this->font_size = _font_size;

            return true;
        }

        void drawText(Vec2 pos, const std::string& text, Align align = Align::LEFT) {
            this->drawText(pos.x, pos.y, text, getRendererColour(this->renderer), align);
        }
        void drawText(float x, float y, const std::string& text, Align align = Align::LEFT) {
            this->drawText(x, y, text, getRendererColour(this->renderer), align);
        }
        void drawText(Vec2 pos, const std::string& text, Colour colour, Align align = Align::LEFT) {
            this->drawText(pos.x, pos.y, text.c_str(), colour, align);
        }
        void drawText(float x, float y, const std::string& text, Colour colour, Align align = Align::LEFT) {
            this->drawText(x, y, text.c_str(), colour, align);
        }

        void drawText(Vec2 pos, const char* text, Align align = Align::LEFT) {
            this->drawText(pos.x, pos.y, text, getRendererColour(this->renderer), align);
        }
        void drawText(float x, float y, const char* text, Align align = Align::LEFT) {
            this->drawText(x, y, text, getRendererColour(this->renderer), align);
        }
        void drawText(Vec2 pos, const char* text, Colour colour, Align align = Align::LEFT) {
            this->drawText(pos.x, pos.y, text, colour, align);
        }
        void drawText(float x, float y, const char* text, Colour colour, Align align = Align::LEFT) {
            SDL_SetTextureColorMod(this->atlas, colour.r, colour.g, colour.b);
            SDL_SetTextureAlphaMod(this->atlas, colour.a);

            float align_x = getAlignX(this->renderer, align, x, this->textWidth(text));

            for (auto i = 0; text[i]; i++) {
                if (text[i] >= 32 && text[i] < 128) {
                    auto info = &this->chars[text[i] - 32];
                    const SDL_Rect src_rect = {info->x0, info->y0, info->x1 - info->x0, info->y1 - info->y0};
                    const SDL_Rect dst_rect = {static_cast<int>(align_x + info->xoff), static_cast<int>(y + info->yoff), info->x1 - info->x0, info->y1 - info->y0};

                    SDL_RenderCopy(this->renderer, this->atlas, &src_rect, &dst_rect);
                    align_x += info->xadvance;
                }
            }
        }

        void drawTextBox(Vec2 pos, const char* text, Colour colour, Colour box_colour, Vec2 padding = Vec2(5,5), bool filled = true, Align align = Align::LEFT) {
            const float width = this->textWidth(text);
            const float height = this->font_size;

            const Vec4 vec4(
                pos.x - padding.x,
                pos.y - padding.y,
                width + (padding.x * 2),
                height + (padding.y * 2));
            drawRect(this->renderer, vec4, box_colour, filled, align);

            this->drawText(Vec2(pos.x, pos.y + (vec4.h / 2)), text, colour, align);
        }
        void drawTextBox(Vec2 pos, const std::string& text, Colour colour, Colour box_colour, Vec2 padding = Vec2(5,5), bool filled = true, Align align = Align::LEFT) {
            this->drawTextBox(pos, text.c_str(), colour, box_colour, padding, filled, align);
        }

        float textWidth(const std::string& text) const {
            return this->textWidth(text.c_str());
        }
        float textWidth(const char *text) const {
            float width = 0;
            for (auto i = 0; text[i]; i++) {
                if (text[i] >= 32 && text[i] < 128) {
                    auto info = &this->chars[text[i] - 32];
                    width += info->xadvance;
                }
            }
            return width;
        }

        int getTextureSize(void) const { return this->texture_size; }
        float getSize(void) const { return this->size; }
        float getScale(void) const { return this->scale; }
        int getAscent(void) const { return this->ascent; }
        int getBaseline(void) const { return this->baseline; }
        bool isOpen(void) const { return this->atlas != nullptr; }

        ~Font() {
            SDL_DestroyTexture(this->atlas);
            if (this->atlas) SDL_DestroyTexture(this->atlas);
            if (this->info) SDL_free(this->info);
            if (this->chars) SDL_free(this->chars);
        }

    protected:
        stbtt_fontinfo* info{nullptr};
        stbtt_packedchar* chars{nullptr};
        SDL_Renderer *renderer{nullptr};
        SDL_Texture* atlas{nullptr};
        int texture_size{0};
        float size{0};
        float scale{0};
        int font_size{0};
        int ascent{0};
        int baseline{0};
};

} // namespace simp
