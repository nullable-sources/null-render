#pragma once
#include "../../internal/shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::directx11 {
	class c_passthrough_color_shader : public backend::i_passthrough_color_shader, public c_shader {
	public:
		c_passthrough_color_shader() : c_shader(&passthrough_color_shader_object, &passthrough_shader_object) { }

	public:
		void use() override;
	};
}