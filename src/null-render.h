//@credits: imgui version - 1.80 WIP (https://github.com/ocornut/imgui/tree/v1.80)
#pragma once

#include <draw-list/draw-list.h>

namespace null::render {
    static void begin_frame(const utils::win::c_window& window) {
        const vec2_t& window_size = window.get_window_size();

        global_atlas.locked = true;
        set_current_font(get_current_font());
        if(!current_font->is_loaded()) throw std::runtime_error{ "!current_font->is_loaded()" };

        shared_data.clip_rect_fullscreen = rect_t{ vec2_t{ 0.f }, window_size };
        shared_data.curve_tessellation_tol = 1.25f;
        shared_data.set_circle_segment_max_error(1.60f);
        shared_data.initialize_flags |= -e_draw_list_flags::anti_aliased_lines | -e_draw_list_flags::anti_aliased_lines_use_texture | -e_draw_list_flags::anti_aliased_fill;

        background_layer.reset_for_begin_frame();
        background_layer.push_texture_id(global_atlas.texture.id);
        background_layer.push_clip_rect_fullscreen();

        foreground_layer.reset_for_begin_frame();
        foreground_layer.push_texture_id(global_atlas.texture.id);
        foreground_layer.push_clip_rect_fullscreen();

        std::ranges::for_each(custom_layers, [](c_draw_list* draw_list) {
            draw_list->reset_for_begin_frame();
            draw_list->push_texture_id(global_atlas.texture.id);
            draw_list->push_clip_rect_fullscreen();
            });

        std::ranges::for_each(fast_layers, [](c_draw_list* draw_list) {
            draw_list->reset_for_begin_frame();
            draw_list->push_texture_id(global_atlas.texture.id);
            draw_list->push_clip_rect_fullscreen();
            }); fast_layers.clear();

        draw_data.window_size = window_size;
        draw_data.layers.clear();
    }

    static void end_frame() {
        global_atlas.locked = false;
    }

    static void setup_draw_data() {
        draw_data.layers.clear();

        if(!background_layer.vtx_buffer.empty()) draw_data.add_draw_list(&background_layer);
        std::ranges::for_each(fast_layers, [](c_draw_list* draw_list) { if(!draw_list->vtx_buffer.empty()) draw_data.add_draw_list(draw_list); });
        std::ranges::for_each(custom_layers, [](c_draw_list* draw_list) { if(!draw_list->vtx_buffer.empty()) draw_data.add_draw_list(draw_list); });
        if(!foreground_layer.vtx_buffer.empty()) draw_data.add_draw_list(&foreground_layer);

        fast_layers.clear();
        draw_data.setup();
    }
}