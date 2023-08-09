#pragma once
#include "shaders/shader.h"
#include "shaders/passthrough-color-shader/compiled-object.h"
#include "shaders/shared-compiled-objects/passthrough-object/compiled-object.h"

namespace null::render::directx9 {
	class c_passthrough_color_shader : public backend::i_passthrough_color_shader, public c_shader {
	public:
		c_passthrough_color_shader() : c_shader{ &passthrough_color_shader_object, &passthrough_shader_object } { }

	public:
		void use() override;
	};
}