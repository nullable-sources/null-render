#pragma once
#include "../passthrough/compiled-object.h"

namespace null::render::opengl3 {
	class c_blur_fragment_shader_object : public c_fragment_shader {
	public:
		memory::resource_t get_source() override {
			return memory::resource_t("null-render:opengl:shaders:blur.frag", "null-render:opengl:resources");
		}
	} inline blur_fragment_shader_object{ };
}