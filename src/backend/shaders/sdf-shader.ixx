module;
#include <memory>
export module null.render:backend.shaders.sdf;

import null.sdk;

import :backend.shader;

export namespace null::render::backend::shaders {
	class i_sdf : public virtual i_shader {
	public:
		virtual void set_aa(const float& aa) = 0;
		virtual void set_outline_thickness(const float& thickness) = 0;
		virtual void set_outline_start(const color_t<int>& color) = 0;
		virtual void set_outline_end(const color_t<int>& color) = 0;
	}; std::unique_ptr<i_sdf> sdf{ };
}