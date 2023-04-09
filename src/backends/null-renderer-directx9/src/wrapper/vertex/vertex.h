#pragma once
#include <d3d9.h>

#include <wrapper/shader/shader.h>

namespace null::renderer::wrapper {
	class c_vertex_shader : public i_shader {
	public:
		IDirect3DVertexShader9* shader{ };

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