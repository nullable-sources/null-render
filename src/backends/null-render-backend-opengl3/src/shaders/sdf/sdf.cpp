#include <shaders/sdf/sdf.h>

namespace null::render::opengl3 {
	void c_sdf_shader::on_create() {
		if(!empty()) return;
		program = std::make_unique<c_program>();
		program->create();

		program->attach_shader(&sdf_shader_object);
		program->attach_shader(&passthrough_shader_object);

		program->link();

		program->detach_shader(&sdf_shader_object);
		program->detach_shader(&passthrough_shader_object);

		matrix.get_location(program.get(), "matrix");

		aa.get_location(program.get(), "aa");
		outline_thickness.get_location(program.get(), "outline_thickness");
		outline_start.get_location(program.get(), "outline_start");
		outline_end.get_location(program.get(), "outline_end");
	}

	void c_sdf_shader::on_destroy() {
		program->destroy();
	}

	void c_sdf_shader::use() {
		if(empty()) return;
		program->use();

		matrix.set(backend::renderer->get_matrix());

		aa.set();
		outline_thickness.set();
		outline_start.set();
		outline_end.set();
	}

	void c_sdf_shader::set_constants(const constants_t& constants) {
		aa.value() = constants.aa;
		outline_thickness.value() = constants.outline_thickness;
		outline_start.value() = constants.outline_start.cast<float>();
		outline_end.value() = constants.outline_end.cast<float>();
	}
}