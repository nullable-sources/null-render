#pragma once
#include "../../internal/shader.h"
#include "compiled-object.h"

namespace null::render::directx11 {
	class c_passthrough_shader : public backend::i_passthrough_shader, public c_shader {
	public:
		c_passthrough_shader() : c_shader(&passthrough_pixel_shader_object, &passthrough_vertex_shader_object) { }
	};
}