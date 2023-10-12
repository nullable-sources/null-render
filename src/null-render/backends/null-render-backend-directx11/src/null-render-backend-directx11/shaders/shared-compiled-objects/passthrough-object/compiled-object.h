#pragma once
#include "../../../wrapper/shaders/vertex-shader.h"

namespace null::render::directx11 {
	namespace sources {
		static const std::vector<byte>& passthrough() {
			#include "compiled/passthrough.h"
			static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
			return source;
		}
	}

	class c_passthrough_shader_object : public c_vertex_shader {
	public:
		struct constant_buffer_t { matrix4x4_t matrix{ }; };
		c_constant_buffer<constant_buffer_t> constant_buffer{ };

	public:
		void set_constant(const constant_buffer_t& constant, int slot = 0) {
			constant_buffer.edit_constant(constant);
			set_constant_buffer(constant_buffer.buffer, slot);
		}

	public:
		void on_create() override {
			if(!empty()) return;
			create(sources::passthrough());
			constant_buffer.create();
		}

		void on_destroy() override { destroy(); constant_buffer.destroy(); }
	} inline passthrough_shader_object{ };
}