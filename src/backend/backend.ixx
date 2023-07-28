module;
#include <memory>
export module null.render:backend;

import null.sdk;

import :backend.renderer;
import :backend.renderer.frame_buffer;
import :backend.renderer.mesh;
import :backend.shaders.passthrough;
import :backend.shaders.quad_gradient;
import :backend.shaders.linear_gradient;
import :backend.shaders.sdf;

export namespace null::render::backend {
	class i_factory {
	public:
		virtual std::unique_ptr<i_renderer> instance_renderer() = 0;
		virtual std::unique_ptr<c_mesh> instance_mesh() = 0;
		virtual std::unique_ptr<i_frame_buffer> instance_frame_buffer(const vec2_t<int>& size, e_frame_buffer_type type, e_frame_buffer_flags flags) = 0;

		virtual std::unique_ptr<shaders::i_passthrough_color> instance_passthrough_color_shader() = 0;
		virtual std::unique_ptr<shaders::i_passthrough_texture> instance_passthrough_texture_shader() = 0;
		virtual std::unique_ptr<shaders::i_quad_gradient> instance_quad_gradient_shader() = 0;
		virtual std::unique_ptr<shaders::i_linear_gradient> instance_linear_gradient_shader() = 0;
		virtual std::unique_ptr<shaders::i_sdf> instance_sdf_shader() = 0;
	}; std::unique_ptr<i_factory> factory{ };
}