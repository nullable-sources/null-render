#pragma once
#include "../../internal/shader/shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::opengl3 {
	class c_linear_gradient_shader : public backend::i_linear_gradient_shader, public c_default_shader {
	public:
		c_uniform<float> angle{ };
		c_uniform<vec2_t<float>> origin{ };
		c_uniform<int> stops_count{ };
		c_uniform<std::array<vec4_t<float>, 16>> colors{ };
		c_uniform<std::array<float, 16>> stops{ };

	public:
		c_linear_gradient_shader() : c_default_shader(&linear_gradient_shader_object, &passthrough_shader_object) { }

	public:
		void create() override;

		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}