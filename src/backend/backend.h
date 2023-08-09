#pragma once
#include "backend/renderer/renderer.h"

#include "backend/shaders/passthrough/passthrough.h"
#include "backend/shaders/quad-gradient/quad-gradient.h"
#include "backend/shaders/linear-gradient/linear-gradient.h"
#include "backend/shaders/sdf/sdf.h"

#include "backend/internal/frame-buffer.h"
#include "backend/internal/mesh.h"

namespace null::render::backend {
	class i_factory {
	public:
		virtual std::unique_ptr<i_renderer> instance_renderer() = 0;
		virtual std::unique_ptr<c_mesh> instance_mesh() = 0;
		virtual std::unique_ptr<i_frame_buffer> instance_frame_buffer(const vec2_t<int>& size, e_frame_buffer_type type, e_frame_buffer_flags flags) = 0;

		virtual std::unique_ptr<i_passthrough_color_shader> instance_passthrough_color_shader() = 0;
		virtual std::unique_ptr<i_passthrough_texture_shader> instance_passthrough_texture_shader() = 0;
		virtual std::unique_ptr<i_quad_gradient_shader> instance_quad_gradient_shader() = 0;
		virtual std::unique_ptr<i_linear_gradient_shader> instance_linear_gradient_shader() = 0;
		virtual std::unique_ptr<i_sdf_shader> instance_sdf_shader() = 0;
	}; inline std::unique_ptr<i_factory> factory{ };
}