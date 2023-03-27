#pragma once
#include <shaders/shader.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <shaders/compiled-objects/passthrough-texture/passthrough-texture.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx11::shaders {
	class c_passthrough_texture : public backend::shaders::i_passthrough_texture, public i_shader {
	public:
		c_passthrough_texture() : i_shader{ &compiled_objects::passthrough_texture, &compiled_objects::passthrough } { }

	public:
		void use() override {
			if(empty()) return;
			i_shader::use();

			compiled_objects::passthrough.set_constant({ renderer->get_matrix() });
		}
	};
}