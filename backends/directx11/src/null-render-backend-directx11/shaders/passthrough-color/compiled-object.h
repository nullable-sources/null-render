#pragma once
#include "../../wrapper/shaders/pixel.h"

namespace null::render::directx11 {
	namespace sources {
		static const std::vector<byte>& passthrough_color() {
			#include "compiled/passthrough-color.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_passthrough_color_shader_object : public c_pixel_shader {
	public:
		void create() override {
			if(!empty()) return;
			compile(sources::passthrough_color());
		}
	} inline passthrough_color_shader_object{ };
}