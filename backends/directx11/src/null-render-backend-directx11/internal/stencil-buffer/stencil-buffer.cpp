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

        if(auto result = shared.device->CreateDepthStencilView(texture, nullptr, &buffer); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "CreateDepthStencilView failed, return code {}.", result);
            return;
        }

        D3D11_BLEND_DESC blend_desc{
            .AlphaToCoverageEnable{ false },
            .RenderTarget{
                {
                    .BlendEnable{ true },
                    .SrcBlend{ D3D11_BLEND_ONE },
                    .DestBlend{ D3D11_BLEND_INV_SRC_ALPHA },
                    .BlendOp{ D3D11_BLEND_OP_ADD },
                    .SrcBlendAlpha{ D3D11_BLEND_ONE },
                    .DestBlendAlpha{ D3D11_BLEND_INV_SRC_ALPHA },
                    .BlendOpAlpha{ D3D11_BLEND_OP_ADD },
                    .RenderTargetWriteMask{ D3D11_COLOR_WRITE_ENABLE_ALL }
                }
        }
        };
        if(auto result = shared.device->CreateBlendState(&blend_desc, &enabled); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant create enabled blend state for stencil buffer, return code {}.", result);

        blend_desc.RenderTarget->RenderTargetWriteMask = 0;
        if(auto result = shared.device->CreateBlendState(&blend_desc, &disabled_rt_write); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant create disabled blend state for stencil buffer, return code {}.", result);

        D3D11_DEPTH_STENCIL_DESC enabled_state_desc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
        enabled_state_desc.DepthEnable = false;
        enabled_state_desc.StencilEnable = true;
        shared.device->CreateDepthStencilState(&enabled_state_desc, &enabled_state);

        D3D11_DEPTH_STENCIL_DESC disabled_state_desc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
        disabled_state_desc.DepthEnable = false;
        disabled_state_desc.StencilEnable = false;
        shared.device->CreateDepthStencilState(&disabled_state_desc, &disabled_state);

        D3D11_DEPTH_STENCIL_DESC set_write_state_desc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
        set_write_state_desc.DepthEnable = false;
        set_write_state_desc.StencilEnable = true;
        set_write_state_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
        set_write_state_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
        shared.device->CreateDepthStencilState(&set_write_state_desc, &set_write_state);

        D3D11_DEPTH_STENCIL_DESC intersect_write_state_desc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
        intersect_write_state_desc.DepthEnable = false;
        intersect_write_state_desc.StencilEnable = true;
        intersect_write_state_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
        intersect_write_state_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
        shared.device->CreateDepthStencilState(&intersect_write_state_desc, &intersect_write_state);

        D3D11_DEPTH_STENCIL_DESC end_write_state_desc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
        end_write_state_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        end_write_state_desc.StencilEnable = true;
        end_write_state_desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
        end_write_state_desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
        shared.device->CreateDepthStencilState(&end_write_state_desc, &end_write_state);
    }

    void c_stencil_buffer::destroy() {
        if(texture) { texture->Release(); texture = nullptr; }
        if(buffer) { buffer->Release(); buffer = nullptr; }
    }

    void c_stencil_buffer::clear() {
        shared.context->ClearDepthStencilView(buffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void c_stencil_buffer::set_test(bool test) {
        if(test) shared.context->OMSetDepthStencilState(enabled_state, 0);
        else shared.context->OMSetDepthStencilState(disabled_state, 0);
    }

    void c_stencil_buffer::set_operation(backend::e_stencil_operation operation) {
        if(operation == backend::e_stencil_operation::end_write) {
            //@note: turning on the geometry rendering
            shared.context->OMSetBlendState(enabled, nullptr, -1);
            shared.context->OMSetDepthStencilState(end_write_state, stencil_ref);
            return;
        }

        //@note: disabling geometry rendering in rt, without this, the mask will be drawn in the same way as regular geometry
        shared.context->OMSetBlendState(disabled_rt_write, nullptr, -1);

        switch(operation) {
            case backend::e_stencil_operation::set:
            case backend::e_stencil_operation::set_inverse: {
                shared.context->OMSetDepthStencilState(set_write_state, 1);
                stencil_ref = operation == backend::e_stencil_operation::set ? 1 : 0;
            } break;

            case backend::e_stencil_operation::intersect: {
                shared.context->OMSetDepthStencilState(intersect_write_state, 1);
                stencil_ref++;
            } break;
        }
    }
}