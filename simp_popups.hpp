// not sure what to call this file yet
// basically its for timed pop-up messages

#pragma once

#include "simp_types.hpp"
#include "simp_helpers.hpp"
#include "simp_font.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <memory>

namespace simp {

struct PopUpEntry {
    std::string text{"oof"};
    Colour text_colour{0xFF,0xFF,0xFF,200};
    Colour box_colour{0,0,0,200};
    Align align{Align::LEFT};
    SDL_TimerID timer_id{0};
};

// this is pretty bad, i think its race conditition also
// i should pass a ref to the font class
// or have an option to make a copy of the font class from a
// created font class
// but im too lazy to do that now so here we are.

// another limitation is that i need all timers to have the same timeout
// there's a few ways to fix it and make it more flexible
// but it works for me heh.
// TODO: fix ^^ tomorrow.
class Popup {
public:
    Popup(SDL_Renderer* renderer, const std::string& font_path, int _font_size) {
        this->font = std::make_unique<Font>(renderer, font_path, _font_size);
        this->font_size = _font_size;
    }
    ~Popup() {
        for (auto& p: this->entries) {
            // timer might not've expired, so we manually remove it
            SDL_RemoveTimer(p.timer_id);
        }
    }

    bool add(const std::string& text) {
        const auto id = SDL_AddTimer(3000, timerCallback, this);
        if (id == 0) {
            return false;
        }
        PopUpEntry entry{};
        entry.text = text;
        entry.timer_id = id;
        this->entries.push_back(entry);
        return true;
    }

    // this looks like 5am code yet its only 6pm.
    void render(void) {
        int y{0};
        for (auto& p: this->entries) {
            // ignoring the rest because, i'll do it tomorrow
            this->font->drawTextBox(Vec2(0, y), p.text,
                p.text_colour, p.box_colour);
            // font + rect_padding + padding
            y += this->font_size + 5 + 5;
        }
    }
private:
    std::unique_ptr<Font> font;
    std::deque<PopUpEntry> entries;
    int y_pos{0};
    int font_size{0};

    static u32 timerCallback(u32 interval, void* param) {
        return static_cast<Popup*>(param)->onTimer(interval);
    }

    // TODO:
    // add a gradual fade after the first timer
    // basically change the alpha from 200 to 0, then pop front.
    // or slide the popup to the left until off screen
    // or both.
    // actually the slide might not work well for *very* long popups
    // probably will just fade it out then...
    u32 onTimer(u32 interval) {
        printf("TIMER\n");
        this->entries.pop_front();
        return 0;
    }
};

} // namespace simp
