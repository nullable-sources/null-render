#pragma once
#include <backend/shaders/passthrough/passthrough.h>
#include <shaders/shader.h>

#include <shaders/compiled-objects/passthrough-texture/passthrough-texture.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::directx9 {
	class c_passthrough_texture_shader : public backend::i_passthrough_texture_shader, public c_shader {
	public:
		c_passthrough_texture_shader() : c_shader{ &passthrough_texture_shader_object, &passthrough_shader_object } { }

	public:
		void use() override;
	};
}