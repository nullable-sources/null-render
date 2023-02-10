#include <renderer/renderer.h>

namespace null::renderer {
    void draw_data_t::add_draw_list(render::c_draw_list* draw_list) {
        if(draw_list && !draw_list->cmd_buffer.empty() && !draw_list->vtx_buffer.empty() && !draw_list->idx_buffer.empty()) {
            draw_lists.push_back(draw_list);
        }
    }
    void draw_data_t::setup() {
        total_vtx_count = total_idx_count = 0;
        std::ranges::for_each(draw_lists, [&](const render::c_draw_list* draw_list) {
            total_vtx_count += draw_list->vtx_buffer.size();
            total_idx_count += draw_list->idx_buffer.size();
            });
    }

    void setup_default_draw_data() {
        draw_data.draw_lists.clear();
        draw_data.add_draw_list(&render::background);
        std::ranges::for_each(render::custom_lists, [](render::c_draw_list* draw_list) { draw_data.add_draw_list(draw_list); });
        std::ranges::for_each(render::fast_lists, [](render::c_draw_list* draw_list) { draw_data.add_draw_list(draw_list); });
        draw_data.add_draw_list(&render::foreground);
        draw_data.setup();

        render::fast_lists.clear();
    }
}