#pragma once
#include "../../wrapper/pixel/pixel.h"

namespace null::render::directx9 {
	namespace sources {
		static const std::vector<byte>& color_vs() {
			#include "compiled/color-vs.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}

		static const std::vector<byte>& color_ps() {
			#include "compiled/color-ps.h"
			static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
			return source;
		}
	}

	class c_color_pixel_shader_object : public c_pixel_shader {
	public:
		virtual const byte* get_source() override { return sources::color_ps().data(); }
	} inline color_pixel_shader_object{ };

	class c_color_vertex_shader_object : public c_vertex_shader {
	public:
		virtual const byte* get_source() override { return sources::color_vs().data(); }
	} inline color_vertex_shader_object{ };
}