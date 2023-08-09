#include <shaders/linear-gradient/linear-gradient.h>

namespace null::render::opengl3 {
	void c_linear_gradient_shader::on_create() {
		if(!empty()) return;
		program = std::make_unique<c_program>();
		program->create();

		program->attach_shader(&linear_gradient_shader_object);
		program->attach_shader(&passthrough_shader_object);

		program->link();

		program->detach_shader(&linear_gradient_shader_object);
		program->detach_shader(&passthrough_shader_object);

		matrix.get_location(program.get(), "matrix");

		angle.get_location(program.get(), "angle");
		stops_count.get_location(program.get(), "stops_count");
		colors.get_location(program.get(), "colors");
		stops.get_location(program.get(), "stops");
	}

	void c_linear_gradient_shader::on_destroy() {
		program->destroy();
	}

	void c_linear_gradient_shader::use() {
		program->use();

		matrix.set(backend::renderer->get_matrix());

		angle.set();
		stops_count.set();
		colors.set();
		stops.set();
	}

	void c_linear_gradient_shader::set_constants(const constants_t& constants) {
		for (auto [color, stop, pair] : std::views::zip(colors.value(), stops.value(), constants.stops)) {
			color = pair.first.cast<float>();
			stop = pair.second;
		}

		stops_count.value() = constants.stops.size();
		angle.value() = constants.angle;
	}
}