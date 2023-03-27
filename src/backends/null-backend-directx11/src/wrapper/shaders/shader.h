#pragma once
#include <wrapper/constant-buffer/constant-buffer.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::directx11::wrapper {
	class i_shader : public backend::shaders::i_compiled_object {
	public:
		virtual void create(const std::vector<std::uint8_t>& source) = 0;
		virtual void destroy() = 0;

		virtual void set() = 0;

		template <typename struct_t>
		void set_constant(const c_constant_buffer<struct_t>& constant_buffer, const int& slot = 0) { set_constant_buffer(constant_buffer.buffer, slot); }
		virtual void set_constant_buffer(ID3D11Buffer* buffer, const int& slot) = 0;

	public:
		virtual void on_destroy() override { destroy(); }
	};
}