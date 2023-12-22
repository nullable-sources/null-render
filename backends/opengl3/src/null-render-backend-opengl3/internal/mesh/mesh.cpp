#include "../../shaders/shared-objects/passthrough/compiled-object.h"
#include "mesh.h"

namespace null::render::opengl3 {
	void c_mesh::create() {
		if(vao != 0) return;

		opengl::gen_vertex_arrays(1, &vao);
		opengl::gen_buffers(1, &vbo);
		opengl::gen_buffers(1, &ibo);
		opengl::bind_vertex_array(vao);

		c_program program{ };
		program.create();

		program.attach_shader(&passthrough_shader_object);
		program.link();
		program.detach_shader(&passthrough_shader_object);

		position.get_location(&program, "position");
		uv.get_location(&program, "uv");
		color.get_location(&program, "color");

		program.destroy();

		opengl::bind_vertex_array(0);
	}

	void c_mesh::destroy() {
		opengl::delete_vertex_arrays(1, &vao);
		opengl::delete_buffers(1, &vbo);
		opengl::delete_buffers(1, &ibo);
		vao = vbo = ibo = 0;
	}

	void c_mesh::compile() {
		if(vao == 0) create();
		opengl::bind_vertex_array(vao);

		opengl::bind_buffer(opengl::e_array_buffer, vbo);
		opengl::buffer_data(opengl::e_array_buffer, geometry_buffer.vertex_buffer.size() * sizeof(backend::vertex_t), (const void*)geometry_buffer.vertex_buffer.data(), opengl::e_static_draw);
		
		position.set();
		uv.set();
		color.set();

		opengl::bind_buffer(opengl::e_element_array_buffer, ibo);
		opengl::buffer_data(opengl::e_element_array_buffer, geometry_buffer.index_buffer.size() * sizeof(backend::index_t), (const void*)geometry_buffer.index_buffer.data(), opengl::e_static_draw);

		opengl::bind_vertex_array(0);
	}

	void c_mesh::use() {
		if(vao == 0) create();
		opengl::bind_vertex_array(vao);
	}
}