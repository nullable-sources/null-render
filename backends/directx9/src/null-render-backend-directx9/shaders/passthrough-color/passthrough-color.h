#pragma once
#include "../../internal/shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::directx9 {
	class c_passthrough_color_shader : public backend::i_passthrough_color_shader, public c_default_shader {
	public:
		c_passthrough_color_shader() : c_default_shader(&passthrough_color_shader_object, &passthrough_shader_object) { }
	};
}