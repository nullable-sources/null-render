#include <wrapper/shaders/shader.h>

namespace null::renderer::wrapper {
	class c_pixel_shader : public i_shader {
	public:
		ID3D11PixelShader* shader{ };

	public:
		void create(const std::vector<std::uint8_t>& source) override {
			auto a = render::directx11->device->CreatePixelShader(source.data(), source.size(), nullptr, &shader);
			int idi_nahui;
		}

		void destroy() override {
			if(empty()) return;
			shader->Release();
			shader = nullptr;
		}

		void set() override {
			render::directx11->context->PSSetShader(shader, nullptr, 0);
		}

		void set_constant_buffer(ID3D11Buffer* buffer, const int& slot) override {
			render::directx11->context->PSSetConstantBuffers(slot, 1, &buffer);
		}

	public:
		bool empty() const override { return !shader; }
	};
}