#pragma once
#include <backend/shaders/shader.h>

namespace null::render::backend {
	class i_passthrough_color_shader : public virtual i_shader { }; inline std::unique_ptr<i_passthrough_color_shader> passthrough_color_shader{ };
	class i_passthrough_texture_shader : public virtual i_shader { }; inline std::unique_ptr<i_passthrough_texture_shader> passthrough_texture_shader{ };
}