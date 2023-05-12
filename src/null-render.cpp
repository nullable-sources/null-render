#include <backend/backend.h>
#include <null-render.h>

namespace null::render {
    void initialize(const utils::win::c_window& window) {
        shared::viewport = window.get_window_size();

        std::ranges::for_each(std::views::iota(0, (int)shared::circle_segments.size()), [=](const int& i) {
            float radius{ i + 1.f };
            shared::circle_segments[i] = std::min(std::clamp((std::numbers::pi * 2.f) / std::acosf((radius - shared::circle_segment_max_error) / radius), 12., 512.), 255.);
            });

        shared::arc_fast_vertex.resize(shared::arc_fast_tessellation_multiplier * 12);
        std::ranges::for_each(std::views::iota(0, (int)shared::arc_fast_vertex.size()), [=](const int& i) {
            float a{ float(i * 2.f * std::numbers::pi) / shared::arc_fast_vertex.size() };
            shared::arc_fast_vertex[i] = vec2_t{ std::cosf(a), std::sinf(a) };
            });

        if(!backend::factory) {
            utils::logger.log(utils::e_log_type::error, "backend factory is empty.");
            return;
        }

        backend::renderer = backend::factory->instance_renderer();
        backend::mesh = backend::factory->instance_mesh();

        //@note: main buffers
        backend::msaa_buffer = backend::factory->instance_frame_buffer(shared::viewport, backend::e_frame_buffer_type::postprocessing, backend::e_frame_buffer_flags::msaa);
        backend::rendering_buffer = backend::factory->instance_frame_buffer(shared::viewport, backend::e_frame_buffer_type::backbuffer, backend::e_frame_buffer_flags::none);
        
        //@note: postprocessing buffers
        backend::rtt_buffer = backend::factory->instance_frame_buffer(shared::viewport, backend::e_frame_buffer_type::postprocessing, backend::e_frame_buffer_flags::none);
        for(std::unique_ptr<backend::i_frame_buffer>& postprocessing_buffer : backend::postprocessing_buffers) {
            postprocessing_buffer = backend::factory->instance_frame_buffer(shared::viewport, backend::e_frame_buffer_type::postprocessing, backend::e_frame_buffer_flags::none);
        }

        backend::shaders::passthrough_color = backend::factory->instance_passthrough_color_shader();
        backend::shaders::passthrough_texture = backend::factory->instance_passthrough_texture_shader();
        backend::shaders::quad_gradient = backend::factory->instance_quad_gradient_shader();
        backend::shaders::linear_gradient = backend::factory->instance_linear_gradient_shader();
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