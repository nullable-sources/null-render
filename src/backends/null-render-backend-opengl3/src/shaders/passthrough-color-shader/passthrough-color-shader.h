#pragma once
#include "shaders/shader.h"
#include "shaders/passthrough-color-shader/compiled-object.h"
#include "shaders/shared-compiled-objects/passthrough-object/compiled-object.h"

namespace null::render::opengl3 {
	class c_passthrough_color_shader : public backend::i_passthrough_color_shader, public c_shader {
	public:
		c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;
	};
}