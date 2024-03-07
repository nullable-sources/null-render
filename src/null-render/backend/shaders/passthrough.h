#pragma once
#include "../internal/shader.h"

namespace null::render::backend {
	class i_passthrough_shader : public virtual i_shader { }; inline std::unique_ptr<i_passthrough_shader> passthrough_shader{ };
	class i_color_shader : public virtual i_shader { }; inline std::unique_ptr<i_color_shader> color_shader{ };
	class i_texture_shader : public virtual i_shader { }; inline std::unique_ptr<i_texture_shader> texture_shader{ };
}