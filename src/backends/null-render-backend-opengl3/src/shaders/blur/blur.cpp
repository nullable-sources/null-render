#include <shaders/blur/blur.h>

namespace null::render::backend::opengl3::shaders {
	void c_blur::on_create() {
		if(!empty()) return;
		program = std::make_unique<wrapper::c_program>();
		program->create();

		program->attach_shader(&compiled_objects::blur);
		program->attach_shader(&compiled_objects::passthrough);

		program->link();

		program->detach_shader(&compiled_objects::blur);
		program->detach_shader(&compiled_objects::passthrough);

		matrix.get_location(program.get(), "matrix");

		amount.get_location(program.get(), "amount");
		direction.get_location(program.get(), "direction");
		texel_size.get_location(program.get(), "texel_size");
		uv_limits.get_location(program.get(), "uv_limits");
	}

	void c_blur::on_destroy() {
		program->destroy();
	}

	void c_blur::use() {
		if(empty()) return;
		program->use();

		matrix.set(renderer->get_matrix());

		amount.set();
		direction.set();
		texel_size.set();
		uv_limits.set();
	}
}