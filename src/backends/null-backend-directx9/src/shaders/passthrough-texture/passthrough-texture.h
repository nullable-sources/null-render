#pragma once
#include <backend/shaders/passthrough/passthrough.h>
#include <shaders/shader.h>

#include <shaders/compiled-objects/passthrough-texture/passthrough-texture.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx9::shaders {
	class c_passthrough_texture : public backend::shaders::i_passthrough_texture, public i_shader {
	public:
		c_passthrough_texture() : i_shader{ &compiled_objects::passthrough_texture, &compiled_objects::passthrough } { }

	public:
		void use() override {
			if(empty()) return;
			i_shader::use();

			vertex_shader->set_constant(0, renderer->get_matrix().linear_array.data(), 4);
		}
	};
}