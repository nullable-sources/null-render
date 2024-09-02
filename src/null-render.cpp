#include "null-render/backend/backend.h"
#include "null-render.h"

namespace ntl::render {
    void initialize(const vec2_t<int>& viewport) {
        shared::viewport = viewport;
        std::ranges::for_each(std::views::iota(0, (int)shared::circle_segments.size()), [=](const int& i) {
            float radius = i + 1.f;
            shared::circle_segments[i] = std::min(std::clamp((std::numbers::pi * 2.f) / std::acosf((radius - shared::circle_segment_max_error) / radius), 12., 512.), 255.);
        });

        shared::arc_fast_vertex.resize(shared::arc_fast_tessellation_multiplier * 12);
        std::ranges::for_each(std::views::iota(0, (int)shared::arc_fast_vertex.size()), [=](const int& i) {
            float a = i * 2.f * std::numbers::pi / shared::arc_fast_vertex.size();
            shared::arc_fast_vertex[i] = vec2_t(std::cosf(a), std::sinf(a));
        });

        if(!backend::factory) {
            sdk::logger(sdk::e_log_type::error, "backend factory is empty.");
            return;
        }

        backend::state_pipeline = backend::factory->instance_state_pipeline();
        backend::default_rasterizer_state = backend::factory->instance_rasterizer_state();
        backend::default_rasterizer_state->unlock();
        backend::default_rasterizer_state->msaa_disable.set(false);
        backend::default_rasterizer_state->scissor_disable.set(false);
        backend::default_rasterizer_state->lock();
        backend::default_blend_state = backend::factory->instance_blend_state();
        backend::default_blend_state->unlock();
        backend::default_blend_state->blend_enable.set(true);
        backend::default_blend_state->src_blend.set(backend::e_blend::one);
        backend::default_blend_state->dst_blend.set(backend::e_blend::inv_src_alpha);
        backend::default_blend_state->src_alpha_blend.set(backend::e_blend::one);
        backend::default_blend_state->dst_alpha_blend.set(backend::e_blend::inv_src_alpha);
        backend::default_blend_state->factor.set(1.f);
        backend::default_blend_state->disable_color_write.set(false);
        backend::default_blend_state->lock();

        backend::renderer = backend::factory->instance_renderer();
        draw_list = c_draw_list::instance(backend::factory->instance_mesh());

        //@note: main buffers
        backend::stencil_buffer = backend::factory->instance_stencil_buffer();
        backend::msaa_buffer = backend::factory->instance_frame_buffer(shared::viewport, backend::e_frame_buffer_type::postprocessing, backend::e_frame_buffer_flags::msaa);
        backend::rendering_buffer = backend::factory->instance_frame_buffer(shared::viewport, backend::e_frame_buffer_type::backbuffer, backend::e_frame_buffer_flags::none);

        backend::post_processing = std::make_unique<backend::c_post_processing>();
        backend::post_processing->initialize();

        backend::passthrough_shader = backend::factory->instance_passthrough_shader();
        backend::color_shader = backend::factory->instance_color_shader();
        backend::texture_shader = backend::factory->instance_texture_shader();
        backend::blur_shader = backend::factory->instance_blur_shader();
        backend::quad_gradient_shader = backend::factory->instance_quad_gradient_shader();
        backend::linear_gradient_shader = backend::factory->instance_linear_gradient_shader();
        backend::radial_gradient_shader = backend::factory->instance_radial_gradient_shader();
        backend::sdf_shader = backend::factory->instance_sdf_shader();
        backend::msdf_shader = backend::factory->instance_msdf_shader();
        backend::mtsdf_shader = backend::factory->instance_mtsdf_shader();
    }

    void begin_frame() {
        backend::renderer->create_objects();

        atlas.locked = true;
        set_default_font(get_default_font());
    }

    void end_frame() {
        atlas.locked = false;
    }
}