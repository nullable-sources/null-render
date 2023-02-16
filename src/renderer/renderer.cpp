#include <renderer/renderer.h>

namespace null::renderer {
    void compiled_geometry_data_t::add_geometry_buffer(render::c_geometry_buffer* geometry_buffer) {
        if(geometry_buffer && !geometry_buffer->cmd_buffer.empty() && !geometry_buffer->vtx_buffer.empty() && !geometry_buffer->idx_buffer.empty()) {
            geometry_buffers.push_back(geometry_buffer);
        }
    }
    void compiled_geometry_data_t::setup() {
        total_vtx_count = total_idx_count = 0;
        std::ranges::for_each(geometry_buffers, [&](const render::c_geometry_buffer* geometry_buffer) {
            total_vtx_count += geometry_buffer->vtx_buffer.size();
            total_idx_count += geometry_buffer->idx_buffer.size();
            });
    }

    void compile_default_geometry_data() {
        compiled_geometry_data.geometry_buffers.clear();
        compiled_geometry_data.add_geometry_buffer(&render::background);
        std::ranges::for_each(render::custom_buffers, [](render::c_geometry_buffer* geometry_buffer) { compiled_geometry_data.add_geometry_buffer(geometry_buffer); });
        std::ranges::for_each(render::fast_buffers, [](render::c_geometry_buffer* geometry_buffer) { compiled_geometry_data.add_geometry_buffer(geometry_buffer); });
        compiled_geometry_data.add_geometry_buffer(&render::foreground);
        compiled_geometry_data.setup();

        render::fast_buffers.clear();
    }
}