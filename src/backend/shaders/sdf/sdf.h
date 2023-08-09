#pragma once
#include <backend/shaders/shader.h>

namespace null::render::backend {
	class i_sdf_shader : public virtual i_shader {
	public:
		struct constants_t {
			color_t<int> outline_start{ }, outline_end{ };
			float aa{ }, outline_thickness{ };
		};

	public:
		virtual void set_constants(const constants_t& constants) = 0;
	}; inline std::unique_ptr<i_sdf_shader> sdf_shader{ };
}