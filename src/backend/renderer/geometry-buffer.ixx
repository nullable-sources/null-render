module;
#include <vector>
export module null.render:backend.renderer.geometry_buffer;

import null.sdk;

export namespace null::render::backend {
	using index_t = std::uint32_t;
	struct vertex_t {
		vec2_t<float> pos{ }, uv{ };
		color_t<int> color{ };
	};

	class c_geometry_buffer {
	public:
		std::vector<vertex_t> vertex_buffer{ };
		std::vector<index_t> index_buffer{ };

	public: //@todo: P0847
		auto& add_index(const index_t& index) { index_buffer.push_back(index); return *this; }
		auto& add_vertex(const vertex_t& vertex) { vertex_buffer.push_back(vertex); return *this; }

	public:
		void clear() { vertex_buffer.clear(); index_buffer.clear(); }
	};
}