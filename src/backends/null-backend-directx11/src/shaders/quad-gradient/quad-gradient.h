#pragma once
#include <backend/shaders/quad-gradient/quad-gradient.h>

#include <shaders/compiled-objects/quad-gradient/quad-gradient.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx11::shaders {
	class c_quad_gradient : public backend::shaders::i_quad_gradient, public i_shader {
	public:
		c_quad_gradient() : i_shader{ &compiled_objects::quad_gradient, &compiled_objects::passthrough } { }

	public:
		void use() override {
			if(empty()) return;
			i_shader::use();

			compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
		}

	public:
		void set_colors(const std::array<color_t<int>, 4>& colors) override {
			compiled_objects::quad_gradient.set_constant({ colors });
		}
	};
}