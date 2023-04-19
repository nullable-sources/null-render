#pragma once
#include <shaders/shader.h>
#include <backend/renderer/renderer.h>
#include <backend/shaders/sdf/sdf.h>

#include <shaders/compiled-objects/sdf/sdf.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::opengl3::shaders {
	class c_sdf : public backend::shaders::i_sdf, public i_shader {
	public:
		wrapper::c_uniform<float> aa{ }, outline_thickness{ };
		wrapper::c_uniform<vec4_t<float>> outline_start{ }, outline_end{ };

		wrapper::c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		void set_aa(const float& _aa) override { aa.value() = _aa; }
		void set_outline_thickness(const float& thickness) override { outline_thickness.value() = thickness; }
		void set_outline_start(const color_t<int>& color) override { outline_start.value() = color.cast<float>(); }
		void set_outline_end(const color_t<int>& color) override { outline_end.value() = color.cast<float>(); }
	};
}