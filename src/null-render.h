//@credits: imgui version - 1.80 WIP (https://github.com/ocornut/imgui/tree/v1.80)
#pragma once
#include <draw-list/draw-list.h>

namespace null::render {
    static void begin_frame(const utils::win::c_window& window) {
        atlas.locked = true;
        c_font::set_current_font(c_font::get_current_font());
        if(!c_font::current_font->is_loaded()) throw std::runtime_error{ "!current_font->is_loaded()" };
        renderer::draw_data_t::screen_size = window.get_window_size();

        background.reset();
        foreground.reset();

        std::ranges::for_each(custom_lists, [](c_draw_list* draw_list) { draw_list->reset(); });
        std::ranges::for_each(fast_lists, [](c_draw_list* draw_list) { draw_list->reset(); });
    }

    static void end_frame() {
        atlas.locked = false;
    }
}