#pragma once
#include "../../wrapper/pixel/pixel.h"

namespace null::render::directx9 {
	namespace sources {
		static const std::vector<byte>& linear_gradient() {
			#include "compiled/linear-gradient.h"
			static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
			return source;
		}
	}

	class c_linear_gradient_shader_object : public c_pixel_shader {
	public:
		void on_create() override {
			if(!empty()) return;
			compile(sources::linear_gradient().data());
		}
	} inline linear_gradient_shader_object{ };
}