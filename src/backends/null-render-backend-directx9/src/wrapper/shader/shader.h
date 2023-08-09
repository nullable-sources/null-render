#pragma once
#include <null-render.h>

namespace null::render::directx9 {
	class i_compiled_shader : public backend::i_compiled_shader_object {
	public:
		virtual void compile(const byte* source) = 0;
		virtual void destroy() = 0;

		virtual void set_constant(int location, const float* params, int count = 1) = 0;
		virtual void set_constant(int location, const int* params, int count = 1) = 0;

	public:
		virtual void set() = 0;

	public:
		virtual void on_destroy() override { destroy(); }
	};
}