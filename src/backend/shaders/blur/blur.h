#pragma once
#include <backend/shaders/shader.h>

namespace null::render::backend::shaders {
	class i_blur : public virtual i_shader {
	public:
		virtual void set_direction(const vec2_t<float>& direction) = 0;
		virtual void set_texel_size(const vec2_t<float>& texel_size) = 0;
		virtual void set_amount(const float& amount) = 0;
		virtual void set_uv_limits(const rect_t<float>& uv_limits) = 0;
	}; inline std::unique_ptr<i_blur> blur{ };
}