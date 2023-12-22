#pragma once
#include "../../wrapper/shader.h"

namespace null::render::opengl3 {
	class c_passthrough_color_shader_object : public c_fragment_shader {
	public:
		memory::resource_t get_source() override {
			return memory::resource_t("null-render:opengl:shaders:passthrough-color.frag", "null-render:opengl:resources");
		}
	} inline passthrough_color_shader_object{ };
}