#pragma once
#include <wrapper/fields/fields.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::opengl3::shaders {
	class i_shader : private virtual backend::shaders::i_shader {
	public:
		std::unique_ptr<wrapper::c_program> program{ };

	public:
		virtual bool empty() const override { return !program || program->empty(); }
	};
}