#pragma once
#include "../shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::opengl3 {
	class c_quad_gradient_shader : public backend::i_quad_gradient_shader, public c_shader {
	public:
		c_uniform<vec4_t<float>> top_left_color{ }, top_right_color{ }, bottom_right_color{ }, bottom_left_color{ };

		c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}