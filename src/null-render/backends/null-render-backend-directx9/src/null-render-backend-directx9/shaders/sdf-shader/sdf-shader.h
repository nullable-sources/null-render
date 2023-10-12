#pragma once
#include "../shader.h"
#include "../shared-compiled-objects/passthrough-object/compiled-object.h"
#include "compiled-object.h"

namespace null::render::directx9 {
	class c_sdf_shader : public backend::i_sdf_shader, public c_shader {
	public:
		c_sdf_shader() : c_shader{ &sdf_shader_object, &passthrough_shader_object } { }

	public:
		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}