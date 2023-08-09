#pragma once
#include <shaders/shader.h>
#include <backend/renderer/renderer.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <shaders/compiled-objects/passthrough-texture/passthrough-texture.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

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