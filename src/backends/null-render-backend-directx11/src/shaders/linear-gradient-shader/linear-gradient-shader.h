#pragma once
#include "shaders/shader.h"
#include "shaders/linear-gradient-shader/compiled-object.h"
#include "shaders/shared-compiled-objects/passthrough-object/compiled-object.h"

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