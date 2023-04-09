#pragma once
#include <backend/shaders/shader.h>

namespace null::render::backend::shaders {
	class i_passthrough_color : public virtual i_shader { }; inline std::unique_ptr<i_passthrough_color> passthrough_color{ };
	class i_passthrough_texture : public virtual i_shader { }; inline std::unique_ptr<i_passthrough_texture> passthrough_texture{ };
}