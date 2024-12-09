#include "stencil-buffer.h"

namespace ntl::render::directx11 {
    void c_stencil_buffer::create() {
        if(!empty()) return;

        D3D11_TEXTURE2D_DESC texture2d_desc{
            .Width{ (std::uint32_t)shared::viewport.x },
            .Height{ (std::uint32_t)shared::viewport.y },
            .MipLevels{ 1 },
            .ArraySize{ 1 },
            .Format{ DXGI_FORMAT_D24_UNORM_S8_UINT },
            .SampleDesc{
                .Count = render::shared::msaa_quality,
                .Quality{ 0 }
            },
            .Usage{ D3D11_USAGE_DEFAULT },
            .BindFlags{ D3D11_BIND_DEPTH_STENCIL },
            .CPUAccessFlags{ 0 }
        };

        if(auto result = shared.device->CreateTexture2D(&texture2d_desc, nullptr, &texture); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "CreateTexture2D failed, return code {}.", result);
            return;
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{
            .Format{ DXGI_FORMAT_D24_UNORM_S8_UINT },
            .ViewDimension{ shared::msaa_quality > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D },
        };

        if(auto result = shared.device->CreateDepthStencilView(texture, &depth_stencil_view_desc, &buffer); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "CreateDepthStencilView failed, return code {}.", result);
            return;
        }
    }

    void c_stencil_buffer::destroy() {
        if(texture) { texture->Release(); texture = nullptr; }
        if(buffer) { buffer->Release(); buffer = nullptr; }
    }

    void c_stencil_buffer::clear() {
        shared.context->ClearDepthStencilView(buffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
}