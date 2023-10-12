#pragma once
#include <null-sdk.h>

namespace null::render::backend {
	using index_t = std::uint32_t;

	struct vertex_t {
		vec2_t<float> pos{ }, uv{ };
		color_t<int> color{ };
	};

	class c_geometry_buffer {
	public:
		std::vector<vertex_t> vertex_buffer{ };
		std::vector<index_t> index_buffer{ };

	public:
		template <typename self_t> auto&& add_index(this self_t&& self, index_t index) { self.index_buffer.push_back(index); return self; }
		template <typename self_t> auto&& add_vertex(this self_t&& self, const vertex_t& vertex) { self.vertex_buffer.push_back(vertex); return self; }

	public:
		void clear() { vertex_buffer.clear(); index_buffer.clear(); }
	};
}