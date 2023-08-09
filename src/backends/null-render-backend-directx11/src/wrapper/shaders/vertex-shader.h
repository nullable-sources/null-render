#pragma once
#include <wrapper/shaders/shader.h>

namespace null::render::directx11 {
	class c_vertex_shader : public i_compiled_shader {
	public:
		ID3D11VertexShader* shader{ };

	public:
		void create(const std::vector<std::uint8_t>& source) override {
			shared.device->CreateVertexShader(source.data(), source.size(), nullptr, &shader);
		}

		void destroy() override {
			if(empty()) return;
			shader->Release();
			shader = nullptr;
		}

		void set() override {
			shared.context->VSSetShader(shader, nullptr, 0);
		}

		void set_constant_buffer(ID3D11Buffer* buffer, int slot) override {
			shared.context->VSSetConstantBuffers(slot, 1, &buffer);
		}

	public:
		bool empty() const override { return !shader; }
	};
}