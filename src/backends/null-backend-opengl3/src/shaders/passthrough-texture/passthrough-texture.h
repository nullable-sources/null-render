#pragma once
#include <shaders/shader.h>
#include <backend/renderer/renderer.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <shaders/compiled-objects/passthrough-texture/passthrough-texture.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::opengl3::shaders {
	class c_passthrough_texture : public backend::shaders::i_passthrough_texture, public i_shader {
	public:
		wrapper::c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;
	};
}