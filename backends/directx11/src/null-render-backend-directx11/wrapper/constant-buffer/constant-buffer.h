#pragma once
#include "../../renderer/renderer.h"

namespace ntl::render::directx11 {
    template <typename struct_t>
    class c_constant_buffer {
    public:
        ID3D11Buffer* buffer{ };

    public:
        virtual void create() {
            D3D11_BUFFER_DESC desc{
                .ByteWidth{ ((sizeof(struct_t) + 15) / 16) * 16 },
                .Usage{ D3D11_USAGE_DYNAMIC },
                .BindFlags{ D3D11_BIND_CONSTANT_BUFFER },
                .CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE }
            };
            if(auto result = shared.device->CreateBuffer(&desc, NULL, &buffer); FAILED(result))
                utils::logger(utils::e_log_type::warning, "cant create constant buffer, return code {}.", result);
        }

        virtual void destroy() { if(buffer) { buffer->Release(); buffer = nullptr; } }

        virtual void edit_constant(const struct_t& constant) {
            struct_t* mapped = map_buffer();
            if(!mapped) return;
            *mapped = constant;
            unmap_buffer();
        }

        virtual struct_t* map_buffer() {
            D3D11_MAPPED_SUBRESOURCE subresource{ };
            if(auto result = shared.context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource); FAILED(result)) {
                utils::logger(utils::e_log_type::warning, "cant map constant buffer, return code {}.", result);
                return nullptr;
            }
            return (struct_t*)subresource.pData;
        }

        virtual void unmap_buffer() {
            shared.context->Unmap(buffer, 0);
        }

    public:
        bool empty() const { return !buffer; }
    };
}