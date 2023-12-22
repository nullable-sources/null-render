#include "radial-gradient.h"

namespace null::render::opengl3 {
	void c_radial_gradient_shader::create() {
		if(!empty()) return;
		c_default_shader::create();

		origin.get_location(program.get(), "origin");
		radius.get_location(program.get(), "radius");
		stops_count.get_location(program.get(), "stops_count");
		colors.get_location(program.get(), "colors");
		stops.get_location(program.get(), "stops");
	}

	void c_radial_gradient_shader::use() {
		if(empty()) return;
		c_default_shader::use();

		origin.set();
		radius.set();
		stops_count.set();
		colors.set();
		stops.set();
	}

	void c_radial_gradient_shader::set_constants(const constants_t& constants) {
		for(auto [color, stop, pair] : std::views::zip(colors.value(), stops.value(), constants.stops)) {
			color = pair.first.cast<float>();
			stop = pair.second;
		}

		stops_count.value() = constants.stops.size();
		origin.value() = constants.origin;
		radius.value() = constants.radius;
	}
}