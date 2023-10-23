#pragma once
#include "../shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::directx11 {
	class c_sdf_shader : public backend::i_sdf_shader, public c_shader {
	public:
		c_sdf_shader() : c_shader(&sdf_shader_object, &passthrough_shader_object) { }

	public:
		void use() override;

	public:
		virtual void set_constants(const constants_t& _constants) override;
	};
}