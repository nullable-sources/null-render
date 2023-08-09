#pragma once
#include <backend/shaders/sdf/sdf.h>
#include <shaders/shader.h>

#include <shaders/compiled-objects/sdf/sdf.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

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