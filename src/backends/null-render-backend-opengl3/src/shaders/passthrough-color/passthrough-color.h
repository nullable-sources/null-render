#pragma once
#include <shaders/shader.h>
#include <backend/renderer/renderer.h>
#include <backend/shaders/passthrough/passthrough.h>

#include <shaders/compiled-objects/passthrough-color/passthrough-color.h>
#include <shaders/compiled-objects/passthrough/passthrough.h>

namespace null::render::backend::opengl3::shaders {
	class c_passthrough_color : public backend::shaders::i_passthrough_color, public i_shader {
	public:
		wrapper::c_uniform<matrix4x4_t> matrix{ };

	public:
		void on_create() override;
		void on_destroy() override;

		void use() override;
	};
}