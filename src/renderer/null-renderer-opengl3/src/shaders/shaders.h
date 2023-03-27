#pragma once
#include <opengl/wrappers/fields/fields.h>
#include <renderer/shaders/shader.h>

namespace null::renderer::shaders {
	class i_shader : private virtual render::impl::shaders::i_shader {
	public:
		std::unique_ptr<renderer::wrapper::c_program> program{ };
		std::unique_ptr<renderer::wrapper::c_fragment_shader> fragment{ };
		std::unique_ptr<renderer::wrapper::c_vertex_shader> vertex{ };

	public:
		virtual bool empty() const override { return (!program || program->empty()) || (!fragment || fragment->empty()) || (!vertex || vertex->empty()); }
	};
}