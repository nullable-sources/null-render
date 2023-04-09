#pragma once
#include <backend/internal/mesh.h>
#include <wrapper/fields/fields.h>

namespace null::render::backend::opengl3 {
	class c_mesh : public i_mesh {
	public:
		wrapper::c_vertex_element<vertex_t, 2, opengl::e_float, false, offsetof(vertex_t, pos)> position{ };
		wrapper::c_vertex_element<vertex_t, 4, opengl::e_int, false, offsetof(vertex_t, color)> color{ };
		wrapper::c_vertex_element<vertex_t, 2, opengl::e_float, false, offsetof(vertex_t, uv)> uv{ };
		std::uint32_t vao{ }, vbo{ }, ibo{ };

	public:
		void create() override;
		void destroy() override;

		void compile() override;

		void set() override;
	};
}