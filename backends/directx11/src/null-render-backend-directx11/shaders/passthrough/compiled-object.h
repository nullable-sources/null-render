#pragma once
#include "../../wrapper/shaders/pixel.h"
#include "../../wrapper/shaders/vertex.h"

namespace null::render::directx11 {
	namespace sources {
		static const std::vector<byte>& passthrough_ps() {
			#include "compiled/passthrough-ps.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}

		static const std::vector<byte>& passthrough_vs() {
			#include "compiled/passthrough-vs.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_passthrough_pixel_shader_object : public c_pixel_shader {
	public:
		void create() override {
			if(!empty()) return;
			compile(sources::passthrough_ps());
		}
	} inline passthrough_pixel_shader_object{ };

	class c_passthrough_vertex_shader_object : public c_vertex_shader {
	public:
		void create() override {
			if(!empty()) return;
			compile(sources::passthrough_vs());
		}
	} inline passthrough_vertex_shader_object{ };
}