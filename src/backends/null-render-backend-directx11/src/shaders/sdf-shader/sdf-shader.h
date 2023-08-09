#pragma once
#include "shaders/shader.h"
#include "shaders/sdf-shader/compiled-object.h"
#include "shaders/shared-compiled-objects/passthrough-object/compiled-object.h"

namespace null::render::directx11 {
	class c_sdf_shader : public backend::i_sdf_shader, public c_shader {
	public:
		c_sdf_shader() : c_shader{ &sdf_shader_object, &passthrough_shader_object } { }

	public:
		void use() override;

	public:
		virtual void set_constants(const constants_t& _constants) override;
	};
}