#pragma once

namespace null::render::backend::directx9::wrapper {
	class i_shader {
	public:
		virtual void compile(const byte* source) = 0;
		virtual void destroy() = 0;

		virtual void set_constant(int location, const float* params, int count = 1) = 0;
		virtual void set_constant(int location, const int* params, int count = 1) = 0;

	public:
		virtual void set() = 0;
		virtual bool empty() const = 0;
	};
}