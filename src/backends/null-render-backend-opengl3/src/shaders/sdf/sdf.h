#pragma once
#include <shaders/shader.h>
#include <backend/renderer/renderer.h>
#include <backend/shaders/sdf/sdf.h>

#include <shaders/compiled-objects/sdf/sdf.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::opengl3 {
	class c_sdf_shader : public backend::i_sdf_shader, public c_shader {
	public:
		c_uniform<float> aa{ }, outline_thickness{ };
		c_uniform<vec4_t<float>> outline_start{ }, outline_end{ };

		c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;

	public:
		void set_constants(const constants_t& constants) override;
	};
}