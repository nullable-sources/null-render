module;
#include <memory>
export module null.render:backend.shaders.passthrough;

import :backend.shader;

export namespace null::render::backend::shaders {
	class i_passthrough_color : public virtual i_shader { }; std::unique_ptr<i_passthrough_color> passthrough_color{ };
	class i_passthrough_texture : public virtual i_shader { }; std::unique_ptr<i_passthrough_texture> passthrough_texture{ };
}