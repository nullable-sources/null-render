#pragma once
#include "../../wrapper/shader.h"

namespace null::render::opengl3 {
	class c_passthrough_texture_shader_object : public c_fragment_shader {
	public:
		memory::resource_t get_source() override {
			return memory::resource_t("null-render:opengl:shaders:passthrough-texture.frag", "null-render:opengl:resources");
		}
	} inline passthrough_texture_shader_object{ };
}