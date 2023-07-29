#pragma once
#include <backend/shaders/sdf/sdf.h>
#include <shaders/shader.h>

#include <shaders/compiled-objects/sdf/sdf.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx9::shaders {
	class c_sdf : public backend::shaders::i_sdf, public i_shader {
	public:
		float aa{ }, outline_thickness{ };
		color_t<float> outline_start{ }, outline_end{ };

	public:
		c_sdf() : i_shader{ &compiled_objects::sdf, &compiled_objects::passthrough } { }

	public:
		void use() override;

	public:
		void set_aa(float _aa) override { aa = _aa; }
		void set_outline_thickness(float thickness) override { outline_thickness = thickness; }
		void set_outline_start(const color_t<int>& color) override { outline_start = color.cast<float>(); }
		void set_outline_end(const color_t<int>& color) override { outline_end = color.cast<float>(); }
	};
}