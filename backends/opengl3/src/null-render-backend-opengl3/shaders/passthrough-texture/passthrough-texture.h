#pragma once
#include "../../internal/shader/shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::opengl3 {
	class c_passthrough_texture_shader : public backend::i_passthrough_texture_shader, public c_default_shader {
	public:
		c_passthrough_texture_shader() : c_default_shader(&passthrough_texture_shader_object, &passthrough_shader_object) { }
	};
}