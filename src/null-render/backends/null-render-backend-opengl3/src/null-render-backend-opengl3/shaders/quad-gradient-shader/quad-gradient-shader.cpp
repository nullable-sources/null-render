#include "quad-gradient-shader.h"

namespace null::render::opengl3 {
	void c_quad_gradient_shader::on_create() {
		if(!empty()) return;
		program = std::make_unique<c_program>();
		program->create();

		program->attach_shader(&quad_gradient_shader_object);
		program->attach_shader(&passthrough_shader_object);

		program->link();

		program->detach_shader(&quad_gradient_shader_object);
		program->detach_shader(&passthrough_shader_object);

		matrix.get_location(program.get(), "matrix");

		top_left_color.get_location(program.get(), "color_tl");
		top_right_color.get_location(program.get(), "color_tr");
		bottom_right_color.get_location(program.get(), "color_br");
		bottom_left_color.get_location(program.get(), "color_bl");
	}

	void c_quad_gradient_shader::on_destroy() {
		program->destroy();
	}

	void c_quad_gradient_shader::use() {
		program->use();

		matrix.set(backend::renderer->get_matrix());

		top_left_color.set();
		top_right_color.set();
		bottom_right_color.set();
		bottom_left_color.set();
	}

	void c_quad_gradient_shader::set_constants(const constants_t& constants) {
		top_left_color.value() = constants.colors[0].cast<float>();
		top_right_color.value() = constants.colors[1].cast<float>();
		bottom_left_color.value() = constants.colors[2].cast<float>();
		bottom_right_color.value() = constants.colors[3].cast<float>();
	}
}