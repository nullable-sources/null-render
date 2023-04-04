//@credits: imgui version - 1.80 WIP (https://github.com/ocornut/imgui/tree/v1.80)
#pragma once
#include <renderer/renderer.h>
#include <font/loaders/freetype/freetype.h>
#include <font/loaders/truetype/truetype.h>
#include <null-sdk.h>

#include <graphic/draw-list/draw-list.h>

#include <graphic/draw-list/brushes/quad-gradient-brush/quad-gradient-brush.h>
#include <graphic/draw-list/text-styles/sdf-text-style/sdf-text-style.h>

namespace null::render {
    namespace shared {
        inline vec2_t<float> viewport{ };
    }

    static void begin_frame(const utils::win::c_window& window) {
        renderer->begin_frame();

        atlas.locked = true;
        set_default_font(get_default_font());
        shared::viewport = window.get_window_size();
    }

    static void end_frame() {
        atlas.locked = false;
        renderer->end_frame();
    }
}