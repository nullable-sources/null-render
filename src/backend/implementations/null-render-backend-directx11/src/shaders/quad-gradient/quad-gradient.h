#pragma once
#include <shaders/shader.h>
#include <backend/shaders/quad-gradient/quad-gradient.h>

#include <shaders/compiled-objects/quad-gradient/quad-gradient.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx11::shaders {
	class c_quad_gradient : public backend::shaders::i_quad_gradient, public i_shader {
	public:
		c_quad_gradient() : i_shader{ &compiled_objects::quad_gradient, &compiled_objects::passthrough } { }

	public:
		void use() override;

	public:
		void set_colors(const std::array<color_t<int>, 4>& colors) override;
	};
}