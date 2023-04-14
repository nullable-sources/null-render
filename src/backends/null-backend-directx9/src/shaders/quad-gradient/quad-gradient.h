#pragma once
#include <backend/shaders/quad-gradient/quad-gradient.h>
#include <shaders/shader.h>

#include <shaders/compiled-objects/quad-gradient/quad-gradient.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx9::shaders {
	class c_quad_gradient : public backend::shaders::i_quad_gradient, public i_shader {
	public:
		std::array<color_t<float>, 4> colors{ };

	public:
		c_quad_gradient() : i_shader{ &compiled_objects::quad_gradient, &compiled_objects::passthrough } { }

	public:
		void use() override {
			if(empty()) return;
			i_shader::use();

			vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);
			pixel_shader->set_constant(0, colors[0].channels.data(), 1);
			pixel_shader->set_constant(1, colors[1].channels.data(), 1);
			pixel_shader->set_constant(2, colors[2].channels.data(), 1);
			pixel_shader->set_constant(3, colors[3].channels.data(), 1);
		}

	public:
		void set_colors(const std::array<color_t<int>, 4>& _colors) override {
			for(std::tuple<color_t<float>&, const color_t<int>&> tuple : std::views::zip(colors, _colors)) {
				std::get<color_t<float>&>(tuple) = std::get<const color_t<int>&>(tuple).cast<float>();
			}
		}
	};
}