#pragma once
#include "../../internal/shader/shader.h"
#include "compiled-object.h"

namespace null::render::opengl3 {
	class c_color_shader : public backend::i_color_shader, public c_default_shader {
	public:
		c_color_shader() : c_default_shader(&color_fragment_shader_object, &color_vertex_shader_object) { }
	};
}