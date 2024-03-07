#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace null::render::directx11 {
	class c_quad_gradient_shader : public backend::i_quad_gradient_shader, public c_shader {
	public:
		c_quad_gradient_shader() : c_shader(&quad_gradient_shader_object, &color_vertex_shader_object) { }

	public:
		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}