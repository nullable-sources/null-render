#pragma once
#include "../shader.h"
#include "../shared-objects/passthrough/compiled-object.h"
#include "compiled-object.h"

namespace null::render::opengl3 {
	class c_passthrough_texture_shader : public backend::i_passthrough_texture_shader, public c_shader {
	public:
		c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;
	};
}