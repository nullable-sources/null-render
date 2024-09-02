#pragma once
#include "shaders/passthrough.h"
#include "shaders/blur.h"
#include "shaders/quad-gradient.h"
#include "shaders/linear-gradient.h"
#include "shaders/radial-gradient.h"
#include "shaders/sdf.h"

#include "internal/stencil-buffer.h"
#include "post-processing/post-processing.h"
#include "renderer/renderer.h"
#include "state-pipeline/state-pipeline.h"

namespace ntl::render::backend {
    class i_factory {
    public:
        virtual std::unique_ptr<i_renderer> instance_renderer() = 0;
        virtual std::unique_ptr<c_mesh> instance_mesh() = 0;
        virtual std::unique_ptr<i_frame_buffer> instance_frame_buffer(const vec2_t<int>& size, e_frame_buffer_type type, e_frame_buffer_flags flags) = 0;
        virtual std::unique_ptr<i_stencil_buffer> instance_stencil_buffer() = 0;

        virtual std::unique_ptr<i_state_pipeline> instance_state_pipeline() = 0;
        virtual std::unique_ptr<i_rasterizer_state> instance_rasterizer_state() = 0;
        virtual std::unique_ptr<i_blend_state> instance_blend_state() = 0;

        virtual std::unique_ptr<i_passthrough_shader> instance_passthrough_shader() = 0;
        virtual std::unique_ptr<i_color_shader> instance_color_shader() = 0;
        virtual std::unique_ptr<i_texture_shader> instance_texture_shader() = 0;
        virtual std::unique_ptr<i_blur_shader> instance_blur_shader() = 0;
        virtual std::unique_ptr<i_quad_gradient_shader> instance_quad_gradient_shader() = 0;
        virtual std::unique_ptr<i_linear_gradient_shader> instance_linear_gradient_shader() = 0;
        virtual std::unique_ptr<i_radial_gradient_shader> instance_radial_gradient_shader() = 0;
        virtual std::unique_ptr<i_sdf_shader> instance_sdf_shader() = 0;
        virtual std::unique_ptr<i_msdf_shader> instance_msdf_shader() = 0;
        virtual std::unique_ptr<i_mtsdf_shader> instance_mtsdf_shader() = 0;
    }; inline std::unique_ptr<i_factory> factory{ };
}