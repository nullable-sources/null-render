#pragma once
#include <wrapper/constant-buffer/constant-buffer.h>
#include <backend/shaders/shader.h>

namespace null::render::directx11 {
	class i_compiled_shader : public backend::i_compiled_shader_object {
	public:
		virtual void create(const std::vector<std::uint8_t>& source) = 0;
		virtual void destroy() = 0;

		virtual void set() = 0;

		template <typename struct_t>
		void set_constant(const c_constant_buffer<struct_t>& constant_buffer, int slot = 0) { set_constant_buffer(constant_buffer.buffer, slot); }
		virtual void set_constant_buffer(ID3D11Buffer* buffer, int slot) = 0;

	public:
		virtual void on_destroy() override { destroy(); }
	};
}