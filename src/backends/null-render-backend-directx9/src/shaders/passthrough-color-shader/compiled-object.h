#pragma once
#include "wrapper/pixel/pixel.h"

namespace null::render::directx9 {
	namespace sources {
		static const std::vector<byte>& passthrough_color() {
			#include "compiled/passthrough-color.h"
			static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
			return source;
		}
	}

	class c_passthrough_color_shader_object : public c_pixel_shader {
	public:
		void on_create() override {
			if(!empty()) return;
			compile(sources::passthrough_color().data());
		}
	} inline passthrough_color_shader_object{ };
}