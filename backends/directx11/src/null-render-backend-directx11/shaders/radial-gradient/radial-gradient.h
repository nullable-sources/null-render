#pragma once
#include "../../internal/shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::directx11 {
	class c_radial_gradient_shader : public backend::i_radial_gradient_shader, public c_shader {
	public:
		c_radial_gradient_shader() : c_shader(&radial_gradient_shader_object, &passthrough_shader_object) { }

	public:
		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}