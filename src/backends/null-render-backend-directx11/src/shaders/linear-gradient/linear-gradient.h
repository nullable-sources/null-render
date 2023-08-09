#pragma once
#include "shaders/shader.h"
#include "shaders/compiled-objects/linear-gradient/linear-gradient.h"
#include "shaders/compiled-objects/passthrough/passthrough.h"

namespace null::render::directx11 {
	class c_linear_gradient_shader : public backend::i_linear_gradient_shader, public c_shader {
	public:
		c_linear_gradient_shader() : c_shader{ &linear_gradient_shader_object, &passthrough_shader_object } { }

	public:
		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}