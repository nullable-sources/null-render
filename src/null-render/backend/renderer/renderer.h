#pragma once
#include "null-render/graphic/draw-list/draw-list.h"

namespace null::render::backend {
	class i_renderer {
	private:
		matrix4x4_t matrix{ };

	public:
		void set_matrix(const matrix4x4_t& _matrix) { matrix = _matrix; }
		const matrix4x4_t& get_matrix() const { return matrix; }

	public:
		virtual matrix4x4_t get_projection_matrix() const = 0;

		virtual void set_texture(void* texture) = 0;
		virtual void set_clip(const rect_t<float>& rect) = 0;
		virtual void draw_geometry(size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) = 0;

		virtual void* create_texture(const vec2_t<float>& size, void* data) = 0;
		virtual void destroy_texture(void* texture) = 0;

		virtual void setup_state() = 0;

	private:
		virtual void save_state() = 0;
		virtual void restore_state() = 0;

		virtual void create_internal_objects() { }
		virtual void destroy_internal_objects() { }

	public:
		void begin_render();
		void end_render();

		void create_objects();
		void destroy_objects();

	public:
		void create_atlases();
		void destroy_atlases();

	public:
		virtual bool empty() const = 0;
	}; inline std::unique_ptr<i_renderer> renderer{ };
}