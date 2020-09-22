#pragma once

#include <SDL2/SDL.h>

namespace simp {

using u8 = unsigned char;
using u32 = unsigned;
using u64 = unsigned long long;
using s64 = long long;

enum class Align {
    LEFT, CENTER, RIGHT
};

struct Vec2 {
    Vec2() : Vec2{0.0f, 0.0f} {}
    Vec2(float _x, float _y) : x{_x}, y{_y} {}
    float x,y;
};

struct Vec4 {
    Vec4() : Vec4{0.0f, 0.0f, 0.0f, 0.0f} {}
    Vec4(const SDL_FRect& rect) : Vec4{rect.x, rect.y, rect.w, rect.h} {}
    Vec4(float _x, float _y, float _w, float _h) : x{_x}, y{_y}, w{_w}, h{_h} {}
    float x, y, w, h;
};

struct Colour {
    Colour() : Colour{0, 0, 0, 0} {}
    Colour(const SDL_Colour& c) : Colour{c.r, c.g, c.b, c.a} {}
    Colour(u8 red, u8 green, u8 blue) : Colour{red, green, blue, 0xFF} {}
    Colour(u8 red, u8 green, u8 blue, u8 alpha) : r{red}, g{green}, b{blue}, a{alpha} {}
    u8 r, g, b, a;
};
using Color = Colour;

struct TextureFormat {
    TextureFormat(u32 _format, int _access, int _w, int _h) : format{_format}, access{_access}, w{_w}, h{_h} {}
    u32 format;
    int access, w, h;
};

struct TextureLocked {
    TextureLocked(void* _pixels, int _pitch, bool _locked) :pixels{_pixels}, pitch{_pitch}, locked{_locked} {}
    void* pixels;
    int pitch;
    bool locked;
};

enum class PixelFormat : u32 {
    UNKOWN = SDL_PIXELFORMAT_UNKNOWN,
    INDEX1LSB = SDL_PIXELFORMAT_INDEX1LSB,
    INDEX1MSB = SDL_PIXELFORMAT_INDEX1MSB,
    INDEX4LSB = SDL_PIXELFORMAT_INDEX4LSB,
    INDEX8 = SDL_PIXELFORMAT_INDEX8,
    RGB332 = SDL_PIXELFORMAT_RGB332,
    RGB444 = SDL_PIXELFORMAT_RGB444,
    BGR444 = SDL_PIXELFORMAT_BGR444,
    RGB555 = SDL_PIXELFORMAT_RGB555,
    BGR555 = SDL_PIXELFORMAT_BGR555,
    ARGB4444 = SDL_PIXELFORMAT_ARGB4444,
    RGBA4444 = SDL_PIXELFORMAT_RGBA4444,
    ABGR4444 = SDL_PIXELFORMAT_ABGR4444,
    BGRA4444 = SDL_PIXELFORMAT_BGRA4444,
    ARGB1555 = SDL_PIXELFORMAT_ARGB1555,
    RGBA5551 = SDL_PIXELFORMAT_RGBA5551,
    ABGR1555 = SDL_PIXELFORMAT_ABGR1555,
    BGRA555 = SDL_PIXELFORMAT_BGRA5551,
    RGB565 = SDL_PIXELFORMAT_RGB565,
    BGR565 = SDL_PIXELFORMAT_BGR565,
    RGB24 = SDL_PIXELFORMAT_RGB24,
    BGR24 = SDL_PIXELFORMAT_BGR24,
    RGB888 = SDL_PIXELFORMAT_RGB888,
    RGBX8888 = SDL_PIXELFORMAT_RGBX8888,
    BGR888 = SDL_PIXELFORMAT_BGR888,
    BGRX8888 = SDL_PIXELFORMAT_BGRX8888,
    ARGB8888 = SDL_PIXELFORMAT_ARGB8888,
    RGBA8888 = SDL_PIXELFORMAT_RGBA8888,
    ABGR8888 = SDL_PIXELFORMAT_ABGR8888,
    BGRA8888 = SDL_PIXELFORMAT_BGRA8888,
    ARGB2101010 = SDL_PIXELFORMAT_ARGB2101010,
    RGBA32 = SDL_PIXELFORMAT_RGBA32,
    ARGB32 = SDL_PIXELFORMAT_ARGB32,
    BGRA32 = SDL_PIXELFORMAT_BGRA32,
    ABGR32 = SDL_PIXELFORMAT_ABGR32,
    YV12 = SDL_PIXELFORMAT_YV12,
    IYUV = SDL_PIXELFORMAT_IYUV,
    YUY2 = SDL_PIXELFORMAT_YUY2,
    UYVY = SDL_PIXELFORMAT_UYVY,
    YVYU = SDL_PIXELFORMAT_YVYU,
    NV12 = SDL_PIXELFORMAT_NV12,
    NV21 = SDL_PIXELFORMAT_NV21,
    OES = SDL_PIXELFORMAT_EXTERNAL_OES
};

enum class WindowFlags : int {
    FULLSCREEN = SDL_WINDOW_FULLSCREEN,
    OPENGL = SDL_WINDOW_OPENGL,
    SHOWN = SDL_WINDOW_SHOWN,
    HIDDEN = SDL_WINDOW_HIDDEN,
    BORDERLESS = SDL_WINDOW_BORDERLESS,
    RESIZABLE = SDL_WINDOW_RESIZABLE,
    MINIMIZED = SDL_WINDOW_MINIMIZED,
    MAXIMIZED = SDL_WINDOW_MAXIMIZED,
    INPUT_GRABBED = SDL_WINDOW_INPUT_GRABBED,
    INPUT_FOCUS = SDL_WINDOW_INPUT_FOCUS,
    MOUSE_FOCUS = SDL_WINDOW_MOUSE_FOCUS,
    FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
    FOREIGN = SDL_WINDOW_FOREIGN,
    ALLOW_HIGHDPI = SDL_WINDOW_ALLOW_HIGHDPI,
    MOUSE_CAPTURE = SDL_WINDOW_MOUSE_CAPTURE,
    ALWAYS_ON_TOP = SDL_WINDOW_ALWAYS_ON_TOP,
    SKIP_TASKBAR = SDL_WINDOW_SKIP_TASKBAR,
    UTILITY = SDL_WINDOW_UTILITY,
    TOOLTIP = SDL_WINDOW_TOOLTIP,
    POPUP_MENU = SDL_WINDOW_POPUP_MENU,
    VULKAN = SDL_WINDOW_VULKAN
};

inline constexpr WindowFlags operator|(WindowFlags a, WindowFlags b) {
    return static_cast<WindowFlags>(static_cast<int>(a) | static_cast<int>(b));
}
inline WindowFlags &operator|=(WindowFlags & a, WindowFlags b) {
    return a = a | b;
}

enum class DisplayOrientation {
    UNKNOWN = SDL_ORIENTATION_UNKNOWN,
    LANDSCAPE = SDL_ORIENTATION_LANDSCAPE,
    LANDSCAPE_FLIPPED = SDL_ORIENTATION_LANDSCAPE_FLIPPED,
    PORTRAIT = SDL_ORIENTATION_PORTRAIT,
    PORTRAIT_FLIPPED = SDL_ORIENTATION_PORTRAIT_FLIPPED
};

enum class GlAttributes {
    RED_SIZE = SDL_GL_RED_SIZE,
    GREEN_SIZE = SDL_GL_GREEN_SIZE,
    BLUE_SIZE = SDL_GL_BLUE_SIZE,
    ALPHA_SIZE = SDL_GL_ALPHA_SIZE,
    BUFFER_SIZE = SDL_GL_BUFFER_SIZE,
    DOUBLEBUFFER = SDL_GL_DOUBLEBUFFER,
    DEPTH_SIZE = SDL_GL_DEPTH_SIZE,
    STENCIL_SIZE = SDL_GL_STENCIL_SIZE,
    ACCUM_RED_SIZE = SDL_GL_ACCUM_RED_SIZE,
    ACCUM_GREEN_SIZE = SDL_GL_ACCUM_GREEN_SIZE,
    ACCUM_BLUE_SIZE = SDL_GL_ACCUM_BLUE_SIZE,
    ACCUM_ALPHA_SIZE = SDL_GL_ACCUM_ALPHA_SIZE,
    STEREO = SDL_GL_STEREO,
    MULTISAMPLEBUFFERS = SDL_GL_MULTISAMPLEBUFFERS,
    MULTISAMPLESAMPLES = SDL_GL_MULTISAMPLESAMPLES,
    ACCELERATED_VISUAL = SDL_GL_ACCELERATED_VISUAL,
    RETAINED_BACKING = SDL_GL_RETAINED_BACKING,
    CONTEXT_MAJOR_VERSION = SDL_GL_CONTEXT_MAJOR_VERSION,
    CONTEXT_MINOR_VERSION = SDL_GL_CONTEXT_MINOR_VERSION,
    CONTEXT_EGL = SDL_GL_CONTEXT_EGL,
    CONTEXT_FLAGS = SDL_GL_CONTEXT_FLAGS,
    CONTEXT_PROFILE_MASK = SDL_GL_CONTEXT_PROFILE_MASK,
    SHARE_WITH_CURRENT_CONTEXT = SDL_GL_SHARE_WITH_CURRENT_CONTEXT,
    FRAMEBUFFER_SRGB_CAPABLE = SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,
    CONTEXT_RELEASE_BEHAVIOR = SDL_GL_CONTEXT_RELEASE_BEHAVIOR,
    CONTEXT_RESET_NOTIFICATION = SDL_GL_CONTEXT_RESET_NOTIFICATION,
    CONTEXT_NO_ERROR = SDL_GL_CONTEXT_NO_ERROR
};

enum class RenderFlags : u32 {
    SOFTWARE = SDL_RENDERER_SOFTWARE,
    ACCELERATED = SDL_RENDERER_ACCELERATED,
    PRESENTVSYNC = SDL_RENDERER_PRESENTVSYNC,
    TARGETTEXTURE = SDL_RENDERER_TARGETTEXTURE
};

inline constexpr RenderFlags operator|(RenderFlags a, RenderFlags b) {
    return static_cast<RenderFlags>(static_cast<u32>(a) | static_cast<u32>(b));
}

enum class ScaleMode {
    NEAREST = SDL_ScaleModeNearest,
    LINEAR = SDL_ScaleModeLinear,
    BEST = SDL_ScaleModeBest
};

enum class TextureAccess : int {
    STATIC = SDL_TEXTUREACCESS_STATIC,
    STREAMING = SDL_TEXTUREACCESS_STREAMING,
    TARGET = SDL_TEXTUREACCESS_TARGET
};

enum class TextureModulate {
    NONE = SDL_TEXTUREMODULATE_NONE,
    COLOUR = SDL_TEXTUREMODULATE_COLOR,
    COLOR = SDL_TEXTUREMODULATE_COLOR,
    ALPHA = SDL_TEXTUREMODULATE_ALPHA
};

enum RenderFlip {
    NONE = SDL_FLIP_NONE,
    HORIZONTAL = SDL_FLIP_HORIZONTAL,
    VERTICAL = SDL_FLIP_VERTICAL
};

enum class BlendMode {
    NONE = SDL_BLENDMODE_NONE,
    BLEND = SDL_BLENDMODE_BLEND,
    ADD = SDL_BLENDMODE_ADD,
    MOD = SDL_BLENDMODE_MOD,
    MUL = SDL_BLENDMODE_MUL,
    INVALID = SDL_BLENDMODE_INVALID
};

} // namespace simp