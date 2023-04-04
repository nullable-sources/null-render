#pragma once
#include <null-sdk.h>

namespace null::render {
	namespace impl {
		using index_t = std::uint32_t;
		struct vertex_t {
			vec2_t<float> pos{ }, uv{ };
			color_t<int> color{ };
		};

		struct geometry_buffer_t {
		public:
			std::vector<vertex_t> vertex_buffer{ };
			std::vector<index_t> index_buffer{ };

		public:
			template <typename self_t> auto&& add_index(this self_t&& self, const index_t& index) { self.index_buffer.push_back(index); return self; }
			template <typename self_t> auto&& add_vertex(this self_t&& self, const vertex_t& vertex) { self.vertex_buffer.push_back(vertex); return self; }

		public:
			void clear() { vertex_buffer.clear(); index_buffer.clear(); }
		};
	}

	class i_renderer {
	private:
		matrix4x4_t matrix{ };

	public:
		impl::geometry_buffer_t geometry_buffer{ };

	public:
		void set_matrix(const matrix4x4_t& _matrix) { matrix = _matrix; }
		const matrix4x4_t& get_matrix() const { return matrix; }

	public:
		virtual void set_texture(void* texture) = 0;
		virtual void set_clip(const rect_t<float>& rect) = 0;
		virtual void draw_geometry(const size_t& vertex_count, const size_t& index_count, const size_t& vertex_offset, const size_t& index_offset) = 0;

	public:
		virtual void initialize() = 0;
		virtual void shutdown() = 0;

		virtual void begin_frame() = 0;
		virtual void end_frame() = 0;

		virtual void begin_render() = 0;
		virtual void end_render() = 0;

		virtual void setup_state() = 0;

		virtual void create_objects() = 0;
		virtual void destroy_objects() = 0;

		virtual void* create_texture(const vec2_t<float>& size, void* data) = 0;
		virtual void destroy_texture(void* texture) = 0;

	public:
		virtual void create_atlases();
		virtual void destroy_atlases();
	}; inline std::unique_ptr<i_renderer> renderer{ }; //note: perhaps need to unify work with shaders and replace the type with std::unique_ptr
}