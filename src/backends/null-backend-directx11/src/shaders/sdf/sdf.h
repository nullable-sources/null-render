#pragma once
#include <backend/shaders/sdf/sdf.h>

#include <shaders/compiled-objects/sdf/sdf.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx11::shaders {
	class c_sdf : public backend::shaders::i_sdf, public i_shader {
	public:
		compiled_objects::c_sdf::constant_buffer_t constant{ };

	public:
		c_sdf() : i_shader{ &compiled_objects::sdf, &compiled_objects::passthrough } { }

	public:
		void use() override {
			if(empty()) return;
			i_shader::use();

			compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
			compiled_objects::sdf.set_constant(constant);
		}

	public:
		virtual void set_aa(const float& aa) override { constant.aa = aa; }
		virtual void set_outline_thickness(const float& thickness) { constant.outline_thickness = thickness; }
		virtual void set_outline_start(const color_t<int>& color) { constant.outline_start = color; }
		virtual void set_outline_end(const color_t<int>& color) { constant.outline_end = color; }
	};
}