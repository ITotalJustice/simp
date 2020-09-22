#pragma once

#include "simp_types.hpp"
#include "simp_primitives.hpp"
#include "simp_helpers.hpp"
//#include "simp_font.hpp"

#include <fstream>
#include <optional>
#include <vector>

#include <SDL2/SDL.h>

namespace simp {

class Fs : public std::ifstream {
    public:
        using std::ifstream::ifstream;
        u64 size() {
            const auto cur_pos = this->tellg();
            if (cur_pos == -1) {
                return 0;
            }
            this->seekg(0, std::ios_base::end);
            const auto size = this->tellg();
            this->seekg(cur_pos, std::ios_base::beg);
            return size;
        }
};


// enum class EntryType { FILE, FOLDER };

// struct FsEntry {
//     FsEntry() = default;
//     FsEntry(const std::string& _name) : name{_name} {}
//     FsEntry(const std::string& _name, EntryType _type) : name{_name}, type{_type} {}
//     std::string name;
//     EntryType type;
//     u64 size;
// };

// struct FsBrowser {
//     std::string last_path;
//     FsEntry selected_entry;
//     bool good;
// };

// #include <dirent.h>

// struct FsDir {
//     FsDir(std::string& path) {
//         this->dir = opendir(path.c_str());
//     }
//     ~FsDir() {
//         closedir(this->dir);
//     }

//     bool read(std::vector<FsEntry>& entries, u64 limit = 100) {
//         if (this->is_open() == false) return false;

//         u64 pos = 0;
//         for (struct dirent* d = nullptr; (d = readdir(this->dir)) && pos < limit; pos++) {
//             FsEntry entry(d->d_name);
//             entries.push_back(entry);
//         }
//     }

//     bool is_open(void) const { return this->dir != nullptr; }

//     DIR* dir{nullptr};
// };

// std::optional<FsBrowser> fileBrowser(SDL_Window* window, SDL_Renderer* renderer, Font& font, const std::string& path) {

//     bool quit = false;
//     std::vector<FsEntry> entries;

//     while (!quit) {
        
//         renderClear(renderer, Colour(0,0,0));

//         u64 pos{0};
//         for (auto &p : entries) {
//             font.drawText(Vec2(10, 10 + (pos * 20)), p.name, Color(0xFF, 0xFF, 0xFF));
//         }

//         SDL_RenderPresent(renderer);
//     }

//     return;
// }

// FsBrowser fileBrowser(const std::string& path) {
//     SDL_Window* window = createWindow("FileBrowser", 640, 480, 0);
//     SDL_Renderer* renderer = createRenderer(window, 0);

//     bool quit = false;
//     while (!quit) {
        
//     }

//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
// }

}
