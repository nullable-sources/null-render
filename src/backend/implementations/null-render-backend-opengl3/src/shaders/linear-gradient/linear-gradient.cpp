#include <shaders/linear-gradient/linear-gradient.h>

namespace null::render::backend::opengl3::shaders {
	void c_linear_gradient::on_create() {
		if(!empty()) return;
		program = std::make_unique<wrapper::c_program>();
		program->create();

		program->attach_shader(&compiled_objects::linear_gradient);
		program->attach_shader(&compiled_objects::passthrough);

		program->link();

		program->detach_shader(&compiled_objects::linear_gradient);
		program->detach_shader(&compiled_objects::passthrough);

		matrix.get_location(program.get(), "matrix");

		angle.get_location(program.get(), "angle");
		stops_count.get_location(program.get(), "stops_count");
		colors.get_location(program.get(), "colors");
		stops.get_location(program.get(), "stops");
	}

	void c_linear_gradient::on_destroy() {
		program->destroy();
	}

	void c_linear_gradient::use() {
		program->use();

		matrix.set(renderer->get_matrix());

		angle.set();
		stops_count.set();
		colors.set();
		stops.set();
	}
}