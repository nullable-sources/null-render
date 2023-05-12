#pragma once
#include <shaders/shader.h>
#include <backend/shaders/blur/blur.h>

#include <shaders/compiled-objects/blur/blur.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx11::shaders {
	class c_blur : public backend::shaders::i_blur, public i_shader {
	public:
		compiled_objects::c_blur::constant_buffer_t constant{ };

	public:
		c_blur() : i_shader{ &compiled_objects::blur, &compiled_objects::passthrough } { }

	public:
		void use() override;

	public:
		void set_direction(const vec2_t<float>& direction) { constant.direction = direction; compiled_objects::blur.set_constant(constant); }
		void set_texel_size(const vec2_t<float>& texel_size) { constant.texel_size = texel_size; }
		void set_amount(const float& amount) { constant.amount = amount; }
		void set_uv_limits(const rect_t<float>& uv_limits) { constant.uv_limits = uv_limits; }
	};
}