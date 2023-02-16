#pragma once
#include <null-renderer-directx11.h>

namespace null::render::shaders {
	namespace sources {
		namespace pixel_sampler {
			#include "pixel-sampler/compiled/pixel-sampler.h"
			static std::vector<std::uint8_t> data() {
				return std::vector<std::uint8_t>{ shader_data, shader_data + sizeof(shader_data) };
			}
		}

		namespace pixel_without_sampler {
			#include "pixel-without-sampler/compiled/pixel-without-sampler.h"
			static std::vector<std::uint8_t> data() {
				return std::vector<std::uint8_t>{ shader_data, shader_data + sizeof(shader_data) };
			}
		}

		namespace vertex {
			#include "vertex/compiled/vertex.h"
			static std::vector<std::uint8_t> data() {
				return std::vector<std::uint8_t>{ shader_data, shader_data + sizeof(shader_data) };
			}
		}
	}
	
	class i_shader {
	public:
		class i_native_wrapper {
		public:
			i_native_wrapper() { }

		public:
			virtual void create(const std::vector<std::uint8_t>& source) = 0;
			virtual void destroy() = 0;

			virtual void set() = 0;

		public:
			virtual operator bool() const = 0;
		};

		template <typename constant_t>
		class i_constant_wrapper {
		public:
			ID3D11Buffer* constant_buffer{ };

		public:
			virtual void create() {
				D3D11_BUFFER_DESC desc{
					.ByteWidth{ ((sizeof(constant_t) + 15) / 16) * 16 },
					.Usage{ D3D11_USAGE_DYNAMIC },
					.BindFlags{ D3D11_BIND_CONSTANT_BUFFER },
					.CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE }
				};
				renderer::directx11->device->CreateBuffer(&desc, NULL, &constant_buffer);
			}

			virtual void destroy() { if(constant_buffer) { constant_buffer->Release(); constant_buffer = nullptr; } }

			virtual void edit_constant(const constant_t& constant) {
				D3D11_MAPPED_SUBRESOURCE mapped_subresource{ };
				renderer::directx11->context->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
				*(constant_t*)mapped_subresource.pData = constant;
				renderer::directx11->context->Unmap(constant_buffer, 0);
			}

			virtual void set() = 0;

		public:
			operator bool() const { return constant_buffer; }
		};

	public:
		virtual void create() { }
		virtual void destroy() { }

		virtual void setup_state() { }

		virtual void begin_frame() { }
		virtual void end_frame() { }

		virtual void set() { }

		virtual void wnd_proc() { }

	public:
		virtual operator bool() const = 0;
	};

	class c_pixel_shader : public i_shader {
	public:
		class c_native_wrapper : public i_native_wrapper {
		public:
			ID3D11PixelShader* shader{ };

		public:
			c_native_wrapper() { }
			c_native_wrapper(const std::vector<std::uint8_t>& source) { create(source); }

		public:
			virtual void create(const std::vector<std::uint8_t>& source) override { if(!shader) renderer::directx11->device->CreatePixelShader(source.data(), source.size(), 0, &shader); }
			virtual void destroy() { if(shader) { shader->Release(); shader = nullptr; } }

			virtual void set() override { if(shader) renderer::directx11->context->PSSetShader(shader, nullptr, 0); }

		public:
			virtual operator bool() const override { return shader; }
		};

		template <typename constant_t>
		class c_constant_wrapper : public i_constant_wrapper<constant_t> {
		public:
			void set() override { renderer::directx11->context->PSSetConstantBuffers(0, 1, &this->constant_buffer); }
		};
	};

	class c_vertex_shader : public i_shader {
	public:
		class c_native_wrapper : public i_native_wrapper {
		public:
			ID3D11VertexShader* shader{ };

		public:
			c_native_wrapper() { }
			c_native_wrapper(const std::vector<std::uint8_t>& source) { create(source); }

		public:
			virtual void create(const std::vector<std::uint8_t>& source) override { if(!shader) renderer::directx11->device->CreateVertexShader(source.data(), source.size(), 0, &shader); }
			virtual void destroy() { if(shader) { shader->Release(); shader = nullptr; } }

			virtual void set() override { if(shader) renderer::directx11->context->VSSetShader(shader, nullptr, 0); }

		public:
			virtual operator bool() const override { return shader; }
		};

		template <typename constant_t>
		class c_constant_wrapper : public i_constant_wrapper<constant_t> {
		public:
			virtual void set() override { renderer::directx11->context->VSSetConstantBuffers(0, 1, &this->constant_buffer); }
		};
	};
}