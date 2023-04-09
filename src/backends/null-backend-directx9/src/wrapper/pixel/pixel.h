#pragma once
#include <renderer/renderer.h>
#include <wrapper/shader/shader.h>

namespace null::render::backend::directx9::wrapper {
	class c_pixel_shader : public i_shader {
	public:
		IDirect3DPixelShader9* shader{ };

	public:
		virtual void compile(const byte* source) override;
		virtual void destroy() override;

		virtual void set_constant(int location, const float* params, int count = 1) override;
		virtual void set_constant(int location, const int* params, int count = 1) override;

	public:
		virtual void set() override;

		virtual bool empty() const override { return !shader; }
	};
}