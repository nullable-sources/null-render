#include "blur.h"

namespace null::render::opengl3 {
	void c_blur_shader::create() {
		if(!empty()) return;
		c_default_shader::create();

		texel_size.get_location(program.get(), "texel_size");
		direction.get_location(program.get(), "direction");
		uv_limits.get_location(program.get(), "uv_limits");
		offsets.get_location(program.get(), "offsets");
		weights.get_location(program.get(), "weights");
		iterations.get_location(program.get(), "iterations");
	}

	void c_blur_shader::use() {
		if(empty()) return;
		c_default_shader::use();

		texel_size.set();
		uv_limits.set();
		offsets.set();
		weights.set();
		iterations.set();
	}

	void c_blur_shader::set_constants(const constants_t& constants) {
		iterations.value() = constants.iterations;
		offsets.value() = constants.offsets;
		weights.value() = constants.weights;
		texel_size.value() = constants.texel_size;
		uv_limits.value() = vec4_t<float>(constants.uv_limits.min.x, 1.f - constants.uv_limits.max.y, constants.uv_limits.max.x, 1.f - constants.uv_limits.min.y);
	}

	void c_blur_shader::set_direction(const vec2_t<float>& _direction) {
		direction.set(_direction);
	}
}