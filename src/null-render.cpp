#include <backend/backend.h>
#include <null-render.h>

namespace null::render {
    void initialize() {
        if(!backend::factory) {
            utils::logger.log(utils::e_log_type::error, "backend factory is empty.");
            return;
        }

        backend::renderer = backend::factory->instance_renderer();
        backend::mesh = backend::factory->instance_mesh();

        backend::msaa_buffer = backend::factory->instance_frame_buffer(backend::e_frame_buffer_type::postprocessing, backend::e_frame_buffer_flags::msaa);
        backend::rendering_buffer = backend::factory->instance_frame_buffer(backend::e_frame_buffer_type::backbuffer, backend::e_frame_buffer_flags::none);
        
        backend::shaders::passthrough_color = backend::factory->instance_passthrough_color_shader();
        backend::shaders::passthrough_texture = backend::factory->instance_passthrough_texture_shader();
        backend::shaders::quad_gradient = backend::factory->instance_quad_gradient_shader();
        backend::shaders::sdf = backend::factory->instance_sdf_shader();
    }

    void begin_frame(const utils::win::c_window& window) {
        shared::viewport = window.get_window_size();
        backend::renderer->create_objects();

        atlas.locked = true;
        set_default_font(get_default_font());
    }

    void end_frame() {
        atlas.locked = false;
    }
}