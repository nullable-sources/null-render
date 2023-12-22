#pragma once
#include "../../wrapper/shaders/pixel.h"

namespace null::render::directx11 {
	namespace sources {
		static const std::vector<byte>& passthrough_texture() {
			#include "compiled/passthrough-texture.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_passthrough_texture_shader_object : public c_pixel_shader {
	public:
		void create() override {
			if(!empty()) return;
			compile(sources::passthrough_texture());
		}
	} inline passthrough_texture_shader_object{ };
}