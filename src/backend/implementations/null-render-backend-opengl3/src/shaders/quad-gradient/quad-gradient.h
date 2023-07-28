#pragma once
#include <shaders/shader.h>
#include <backend/renderer/renderer.h>
#include <backend/shaders/quad-gradient/quad-gradient.h>

#include <shaders/compiled-objects/quad-gradient/quad-gradient.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::opengl3::shaders {
	class c_quad_gradient : public backend::shaders::i_quad_gradient, public i_shader {
	public:
		wrapper::c_uniform<vec4_t<float>> top_left_color{ }, top_right_color{ }, bottom_right_color{ }, bottom_left_color{ };

		wrapper::c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		void set_colors(const std::array<color_t<int>, 4>& colors) override {
			top_left_color.value() = colors[0].cast<float>();
			top_right_color.value() = colors[1].cast<float>();
			bottom_left_color.value() = colors[2].cast<float>();
			bottom_right_color.value() = colors[3].cast<float>();
		}
	};
}