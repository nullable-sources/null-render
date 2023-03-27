#include <shaders/passthrough-color/passthrough-color.h>

namespace null::render::backend::opengl3::shaders {
	void c_passthrough_color::on_create() {
		if(!empty()) return;
		program = std::make_unique<wrapper::c_program>();
		program->create();

		program->attach_shader(&compiled_objects::passthrough_color);
		program->attach_shader(&compiled_objects::passthrough);

		program->link();

		program->detach_shader(&compiled_objects::passthrough_color);
		program->detach_shader(&compiled_objects::passthrough);

		matrix.get_location(program.get(), "matrix");
	}

	void c_passthrough_color::on_destroy() {
		program->destroy();
	}

	void c_passthrough_color::use() {
		program->use();

		matrix.set(renderer->get_matrix());
	}
}