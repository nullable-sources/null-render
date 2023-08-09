#pragma once
#include <shaders/shader.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <shaders/compiled-objects/passthrough-color/passthrough-color.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::directx11 {
	class c_passthrough_color_shader : public backend::i_passthrough_color_shader, public c_shader {
	public:
		c_passthrough_color_shader() : c_shader{ &passthrough_color_shader_object, &passthrough_shader_object } { }

	public:
		void use() override;
	};
}