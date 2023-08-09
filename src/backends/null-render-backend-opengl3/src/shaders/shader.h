#pragma once
#include <backend/shaders/shader.h>
#include <wrapper/fields.h>

namespace null::render::opengl3 {
	class c_shader : private virtual backend::i_shader {
	public:
		std::unique_ptr<c_program> program{ };

	public:
		virtual bool empty() const override { return !program || program->empty(); }
	};
}