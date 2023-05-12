#include <shaders/quad-gradient/quad-gradient.h>

namespace null::render::backend::opengl3::shaders {
	void c_quad_gradient::on_create() {
		if(!empty()) return;
		program = std::make_unique<wrapper::c_program>();
		program->create();

		program->attach_shader(&compiled_objects::quad_gradient);
		program->attach_shader(&compiled_objects::passthrough);

		program->link();

		program->detach_shader(&compiled_objects::quad_gradient);
		program->detach_shader(&compiled_objects::passthrough);

		matrix.get_location(program.get(), "matrix");

		top_left_color.get_location(program.get(), "color_tl");
		top_right_color.get_location(program.get(), "color_tr");
		bottom_right_color.get_location(program.get(), "color_br");
		bottom_left_color.get_location(program.get(), "color_bl");
	}

	void c_quad_gradient::on_destroy() {
		program->destroy();
	}

	void c_quad_gradient::use() {
		program->use();

		matrix.set(renderer->get_matrix());

		top_left_color.set();
		top_right_color.set();
		bottom_right_color.set();
		bottom_left_color.set();
	}
}