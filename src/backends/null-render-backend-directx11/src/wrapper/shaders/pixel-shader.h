#pragma once
#include <wrapper/shaders/shader.h>

namespace null::render::directx11 {
	class c_pixel_shader : public i_compiled_shader {
	public:
		ID3D11PixelShader* shader{ };

	public:
		void create(const std::vector<std::uint8_t>& source) override {
			shared.device->CreatePixelShader(source.data(), source.size(), nullptr, &shader);
		}

		void destroy() override {
			if(empty()) return;
			shader->Release();
			shader = nullptr;
		}

		void set() override {
			shared.context->PSSetShader(shader, nullptr, 0);
		}

		void set_constant_buffer(ID3D11Buffer* buffer, int slot) override {
			shared.context->PSSetConstantBuffers(slot, 1, &buffer);
		}

	public:
		bool empty() const override { return !shader; }
	};
}