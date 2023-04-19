#include <shaders/sdf/sdf.h>

namespace null::render::backend::opengl3::shaders {
	void c_sdf::on_create() {
		if(!empty()) return;
		program = std::make_unique<wrapper::c_program>();
		program->create();

		program->attach_shader(&compiled_objects::sdf);
		program->attach_shader(&compiled_objects::passthrough);

		program->link();

		program->detach_shader(&compiled_objects::sdf);
		program->detach_shader(&compiled_objects::passthrough);

		matrix.get_location(program.get(), "matrix");

		aa.get_location(program.get(), "aa");
		outline_thickness.get_location(program.get(), "outline_thickness");
		outline_start.get_location(program.get(), "outline_start");
		outline_end.get_location(program.get(), "outline_end");
	}

	void c_sdf::on_destroy() {
		program->destroy();
	}

	void c_sdf::use() {
		if(empty()) return;
		program->use();

		matrix.set(renderer->get_matrix());

		aa.set();
		outline_thickness.set();
		outline_start.set();
		outline_end.set();
	}
}