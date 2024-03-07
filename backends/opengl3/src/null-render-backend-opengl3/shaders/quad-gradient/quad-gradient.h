#pragma once
#include "../../internal/shader/shader.h"
#include "compiled-object.h"

namespace null::render::opengl3 {
	class c_quad_gradient_shader : public backend::i_quad_gradient_shader, public c_default_shader {
	public:
		c_uniform<vec4_t<float>> top_left_color{ }, top_right_color{ }, bottom_right_color{ }, bottom_left_color{ };

	public:
		c_quad_gradient_shader() : c_default_shader(&quad_gradient_shader_object, &color_vertex_shader_object) { }

	public:
		void create() override;

		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}