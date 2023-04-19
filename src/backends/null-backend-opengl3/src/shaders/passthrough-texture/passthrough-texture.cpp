#include <shaders/passthrough-texture/passthrough-texture.h>

namespace null::render::backend::opengl3::shaders {
	void c_passthrough_texture::on_create() {
		if(!empty()) return;
		program = std::make_unique<wrapper::c_program>();
		program->create();

		program->attach_shader(&compiled_objects::passthrough_texture);
		program->attach_shader(&compiled_objects::passthrough);

		program->link();

		program->detach_shader(&compiled_objects::passthrough_texture);
		program->detach_shader(&compiled_objects::passthrough);

		matrix.get_location(program.get(), "matrix");
	}

	void c_passthrough_texture::on_destroy() {
		program->destroy();
	}

	void c_passthrough_texture::use() {
		program->use();

		matrix.set(renderer->get_matrix());
	}
}