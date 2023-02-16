//@credits: imgui version - 1.80 WIP (https://github.com/ocornut/imgui/tree/v1.80)
#pragma once
#include <renderer/renderer.h>

namespace null::render {
    namespace shared {
        inline vec2_t<float> viewport{ };
    }

    static void begin_frame(const utils::win::c_window& window) {
        renderer::renderer->begin_frame();

        atlas.locked = true;
        c_font::set_current_font(c_font::get_current_font());
        if(!c_font::current_font->is_loaded()) throw std::runtime_error{ "!current_font->is_loaded()" };
        shared::viewport = window.get_window_size();

        background.reset();
        foreground.reset();

        std::ranges::for_each(custom_buffers, [](c_geometry_buffer* geometry_buffer) { geometry_buffer->reset(); });
        std::ranges::for_each(fast_buffers, [](c_geometry_buffer* geometry_buffer) { geometry_buffer->reset(); });
    }

    static void end_frame() {
        atlas.locked = false;
        renderer::renderer->end_frame();
    }
}