#pragma once
#include "shaders/shader.h"
#include "shaders/compiled-objects/linear-gradient/linear-gradient.h"
#include "shaders/compiled-objects/passthrough/passthrough.h"

namespace null::render::opengl3 {
	class c_linear_gradient_shader : public backend::i_linear_gradient_shader, public c_shader {
	public:
		c_uniform<float> angle{ };
		c_uniform<int> stops_count{ };
		c_uniform<std::array<vec4_t<float>, 16>> colors{ };
		c_uniform<std::array<float, 16>> stops{ };

		c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}