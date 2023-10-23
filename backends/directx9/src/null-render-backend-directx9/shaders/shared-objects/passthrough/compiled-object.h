#pragma once
#include "../../../wrapper/vertex/vertex.h"

namespace null::render::directx9 {
	namespace sources {
		static const std::vector<byte>& passthrough() {
			#include "compiled/passthrough.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_passthrough_shader_object : public c_vertex_shader {
	public:
		void on_create() override {
			if(!empty()) return;
			compile(sources::passthrough().data());
		}
	} inline passthrough_shader_object{ };
}