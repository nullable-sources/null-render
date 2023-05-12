#pragma once
#include <backend/shaders/blur/blur.h>
#include <shaders/shader.h>

#include <shaders/compiled-objects/blur/blur.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::directx9::shaders {
	class c_blur : public backend::shaders::i_blur, public i_shader {
	public:
		float amount{ };
		vec2_t<float> direction{ }, texel_size{ };
		rect_t<float> uv_limits{ };

	public:
		c_blur() : i_shader{ &compiled_objects::blur, &compiled_objects::passthrough } { }

	public:
		void use() override;

	public:
		void set_direction(const vec2_t<float>& _direction) { direction = _direction; }
		void set_texel_size(const vec2_t<float>& _texel_size) { texel_size = _texel_size; }
		void set_amount(const float& _amount) { amount = _amount; }
		void set_uv_limits(const rect_t<float>& _uv_limits) { uv_limits = _uv_limits; }
	};
}