#pragma once
#include "../../internal/shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::directx9 {
	class c_radial_gradient_shader : public backend::i_radial_gradient_shader, public c_default_shader {
	public:
		c_radial_gradient_shader() : c_default_shader(&radial_gradient_shader_object, &passthrough_shader_object) { }

	public:
		void set_constants(const constants_t& constants) override;
	};
}