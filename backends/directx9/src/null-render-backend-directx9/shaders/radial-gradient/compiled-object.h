#pragma once
#include "../../wrapper/pixel/pixel.h"

namespace null::render::directx9 {
	namespace sources {
		static const std::vector<byte>& radial_gradient() {
			#include "compiled/radial-gradient.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_radial_gradient_shader_object : public c_pixel_shader {
	public:
		virtual const byte* get_source() override { return sources::radial_gradient().data(); }
	} inline radial_gradient_shader_object{ };
}