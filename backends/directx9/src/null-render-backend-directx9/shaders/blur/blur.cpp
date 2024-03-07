#include "blur.h"

namespace null::render::directx9 {
	void c_blur_shader::set_direction(const vec2_t<float>& _direction) {
		pixel_shader->set_constant(1, (float*)&_direction);
	}

	void c_blur_shader::set_constants(const constants_t& constants) {
		std::array<vec4_t<float>, 24> offsets_packed{ };
		std::array<vec4_t<float>, 24> weights_packed{ };

		for(auto [chunk_index, chunk] : std::views::zip(constants.weights, constants.offsets) | std::views::take(constants.iterations) | std::views::chunk(4) | std::views::enumerate) {
			for(auto [element_index, chunk_element] : chunk | std::views::enumerate) {
				weights_packed[chunk_index][element_index] = std::get<0>(chunk_element);
				offsets_packed[chunk_index][element_index] = std::get<1>(chunk_element);
			}
		}

		float iterations = constants.iterations;
		pixel_shader->set_constant(0, &iterations);
		pixel_shader->set_constant(2, constants.texel_size.coordinates.data());
		pixel_shader->set_constant(3, constants.uv_limits.linear_corners.data());
		pixel_shader->set_constant(4, (float*)offsets_packed.data(), 24);
		pixel_shader->set_constant(28, (float*)weights_packed.data(), 24);
	}
}