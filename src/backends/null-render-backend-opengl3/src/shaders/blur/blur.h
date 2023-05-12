#pragma once
#include <shaders/shader.h>
#include <backend/renderer/renderer.h>
#include <backend/shaders/blur/blur.h>

#include <shaders/compiled-objects/blur/blur.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::opengl3::shaders {
	class c_blur : public backend::shaders::i_blur, public i_shader {
	public:
		wrapper::c_uniform<float> amount{ };
		wrapper::c_uniform<vec2_t<float>> direction{ }, texel_size{ };
		wrapper::c_uniform<vec4_t<float>> uv_limits{ };

		wrapper::c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		virtual void set_direction(const vec2_t<float>& _direction) { direction.set(_direction); }
		virtual void set_texel_size(const vec2_t<float>& _texel_size) { texel_size.value() = _texel_size; }
		virtual void set_amount(const float& _amount) { amount.value() = _amount; }
		virtual void set_uv_limits(const rect_t<float>& _uv_limits) { uv_limits.value() = vec4_t<float>{ _uv_limits.min.x, _uv_limits.min.y, _uv_limits.max.x, _uv_limits.max.y }; }
	};
}