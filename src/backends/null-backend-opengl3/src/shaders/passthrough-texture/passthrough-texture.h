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
		void on_create() override {
			if(!empty()) return;
			program = std::make_unique<wrapper::c_program>();
			program->create();

			program->attach_shader(&compiled_objects::passthrough_texture);
			program->attach_shader(&compiled_objects::passthrough);

			program->link();

			program->detach_shader(&compiled_objects::passthrough_texture);
			program->detach_shader(&compiled_objects::passthrough);

			matrix.get_location(program.get(), "matrix");
		}

		void on_destroy() override {
			program->destroy();
		}

		void use() override {
			program->use();

			matrix.set(renderer->get_matrix());
		}
	};
}