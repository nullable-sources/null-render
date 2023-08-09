#pragma once
#include <shaders/shader.h>
#include <backend/shaders/quad-gradient/quad-gradient.h>

#include <shaders/compiled-objects/quad-gradient/quad-gradient.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::directx11 {
	class c_quad_gradient_shader : public backend::i_quad_gradient_shader, public c_shader {
	public:
		c_quad_gradient_shader() : c_shader{ &quad_gradient_shader_object, &passthrough_shader_object } { }

	public:
		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}