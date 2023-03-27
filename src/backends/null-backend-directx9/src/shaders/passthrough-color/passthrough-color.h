#pragma once
#include <backend/shaders/passthrough/passthrough.h>
#include <shaders/shader.h>

#include <shaders/compiled-objects/passthrough-color/passthrough-color.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx9::shaders {
	class c_passthrough_color : public backend::shaders::i_passthrough_color, public i_shader {
	public:
		c_passthrough_color() : i_shader{ &compiled_objects::passthrough_color, &compiled_objects::passthrough } { }

	public:
		void use() override {
			if(empty()) return;
			i_shader::use();

			vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);
		}
	};
}