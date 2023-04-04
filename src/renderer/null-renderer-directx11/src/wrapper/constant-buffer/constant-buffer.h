#pragma once
#include <null-renderer-directx11.h>

namespace null::renderer::wrapper {
	template <typename struct_t>
	class c_constant_buffer {
	public:
		ID3D11Buffer* buffer{ };

	public:
		virtual void create() {
			D3D11_BUFFER_DESC desc{
				/*.ByteWidth{ std::uint32_t(std::ceil((sizeof(struct_t) + 15) / 16.f) * 16) },*/
				.ByteWidth{ ((sizeof(struct_t) + 15) / 16) * 16 },
				.Usage{ D3D11_USAGE_DYNAMIC },
				.BindFlags{ D3D11_BIND_CONSTANT_BUFFER },
				.CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE }
			};
			if(auto result{ render::directx11->device->CreateBuffer(&desc, NULL, &buffer) }; FAILED(result))
				utils::logger.log(utils::e_log_type::warning, "cant create constant buffer, return code {}.", result);
		}

		virtual void destroy() { if(buffer) { buffer->Release(); buffer = nullptr; } }

		virtual void edit_constant(const struct_t& constant) {
			D3D11_MAPPED_SUBRESOURCE subresource{ };
			if(auto result{ render::directx11->context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource) }; FAILED(result)) {
				utils::logger.log(utils::e_log_type::warning, "cant map constant buffer, return code {}.", result);
				return;
			}
			*(struct_t*)subresource.pData = constant;
			render::directx11->context->Unmap(buffer, 0);
		}

	public:
		bool empty() const { return !buffer; }
	};
}