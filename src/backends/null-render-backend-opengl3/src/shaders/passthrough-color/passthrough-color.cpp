#include "shaders/passthrough-color/passthrough-color.h"

namespace null::render::opengl3 {
	void c_passthrough_color_shader::on_create() {
		if(!empty()) return;
		program = std::make_unique<c_program>();
		program->create();

		program->attach_shader(&passthrough_color_shader_object);
		program->attach_shader(&passthrough_shader_object);

		program->link();

		program->detach_shader(&passthrough_color_shader_object);
		program->detach_shader(&passthrough_shader_object);

		matrix.get_location(program.get(), "matrix");
	}

	void c_passthrough_color_shader::on_destroy() {
		program->destroy();
	}

	void c_passthrough_color_shader::use() {
		program->use();

		matrix.set(backend::renderer->get_matrix());
	}
}