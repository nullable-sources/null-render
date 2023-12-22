#include "sdf.h"

namespace null::render::opengl3 {
	void c_sdf_shader::create() {
		if(!empty()) return;
		c_default_shader::create();

		aa.get_location(program.get(), "aa");
		outline_thickness.get_location(program.get(), "outline_thickness");
		outline_start.get_location(program.get(), "outline_start");
		outline_end.get_location(program.get(), "outline_end");
	}

	void c_sdf_shader::use() {
		if(empty()) return;
		c_default_shader::use();

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