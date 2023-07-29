#include <null-render.h>

#include <internal/frame-buffer/frame-buffer.h>

namespace null::render::backend::directx11 {
	void c_frame_buffer::on_create() {
        if(!empty()) return;

        //@note: creating texture for render target
        if(type == e_frame_buffer_type::postprocessing) {
            D3D11_TEXTURE2D_DESC texture2d_desc{
               .Width{ (std::uint32_t)size.x },
               .Height{ (std::uint32_t)size.y },
               .MipLevels{ 1 },
               .ArraySize{ 1 },
               .Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
               .SampleDesc{
                   .Count{ flags & e_frame_buffer_flags::msaa ? render::shared::msaa_quality : 1 },
                   .Quality{ 0 }
               },
               .Usage{ D3D11_USAGE_DEFAULT },
               .BindFlags{ D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE },
               .CPUAccessFlags{ 0 }
            };

            if(auto result{ shared.device->CreateTexture2D(&texture2d_desc, nullptr, &render_target_texture) }; FAILED(result)) {
                utils::logger(utils::e_log_type::error, "CreateTexture2D failed, return code {}.", result);
                return;
            }
        } else {
            if(auto result{ shared.swap_chain->GetBuffer(0, IID_PPV_ARGS(&render_target_texture)) }; FAILED(result)) {
                utils::logger(utils::e_log_type::error, "GetBuffer failed, return code {}.", result);
                return;
            }
        }

        //@note: creating render target
        D3D11_TEXTURE2D_DESC render_target_texture_desc{ };
        render_target_texture->GetDesc(&render_target_texture_desc); //@note: backbuffer can also be with msaa
        D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc{
            .Format{ render_target_texture_desc.Format },
            .ViewDimension{ render_target_texture_desc.SampleDesc.Count > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D },
        };

        if(auto result{ shared.device->CreateRenderTargetView(render_target_texture, &render_target_view_desc, &render_target) }; FAILED(result)) {
            utils::logger(utils::e_log_type::error, "CreateRenderTargetView failed, return code {}.", result);
            return;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{
            .Format{ render_target_texture_desc.Format },
            .ViewDimension{ render_target_texture_desc.SampleDesc.Count > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D },
            .Texture2D{
                .MostDetailedMip{ 0 },
                .MipLevels{ 1 }
            }
        };
        if(auto result{ shared.device->CreateShaderResourceView(render_target_texture, &shader_resource_view_desc, &shader_resource_view) }; FAILED(result)) {
            utils::logger(utils::e_log_type::error, "CreateShaderResourceView failed, return code {}.", result);
            return;
        }

        D3D11_RASTERIZER_DESC rasterizer_desc{
                .FillMode{ D3D11_FILL_SOLID },
                .CullMode{ D3D11_CULL_NONE },
                .DepthClipEnable{ true },
                .ScissorEnable{ true },
                .MultisampleEnable{ render_target_texture_desc.SampleDesc.Count > 1 }
        };
        if(auto result{ shared.device->CreateRasterizerState(&rasterizer_desc, &rasterizer_state) }; FAILED(result)) {
            utils::logger(utils::e_log_type::error, "CreateRasterizerState failed, return code {}.", result);
            return;
        }

        D3D11_TEXTURE2D_DESC depth_stencil_view_texture2d_desc{
               .Width{ render_target_texture_desc.Width },
               .Height{ render_target_texture_desc.Height },
               .MipLevels{ 1 },
               .ArraySize{ 1 },
               .Format{ DXGI_FORMAT_D32_FLOAT },
               .SampleDesc{ render_target_texture_desc.SampleDesc },
               .Usage{ D3D11_USAGE_DEFAULT },
               .BindFlags{ D3D11_BIND_DEPTH_STENCIL },
               .CPUAccessFlags{ 0 }
        };

        if(auto result{ shared.device->CreateTexture2D(&depth_stencil_view_texture2d_desc, nullptr, &depth_stencil_view_texture) }; FAILED(result)) {
            utils::logger(utils::e_log_type::error, "CreateTexture2D failed, return code {}.", result);
            return;
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{
            .Format{ DXGI_FORMAT_D32_FLOAT },
            .ViewDimension{ render_target_texture_desc.SampleDesc.Count > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D },
        };

        if(auto result{ shared.device->CreateDepthStencilView(depth_stencil_view_texture, &depth_stencil_view_desc, &depth_stencil_view) }; FAILED(result)) {
            utils::logger(utils::e_log_type::error, "CreateDepthStencilView failed, return code {}.", result);
            return;
        }
	}

	void c_frame_buffer::on_destroy() {
        if(rasterizer_state) { rasterizer_state->Release(); rasterizer_state = nullptr; }
        if(render_target_texture) { render_target_texture->Release(); render_target_texture = nullptr; }
        if(depth_stencil_view_texture) { depth_stencil_view_texture->Release(); depth_stencil_view_texture = nullptr; }
        if(shader_resource_view) { shader_resource_view->Release(); shader_resource_view = nullptr; }
        if(render_target) { render_target->Release(); render_target = nullptr; }
        if(depth_stencil_view) { depth_stencil_view->Release(); depth_stencil_view = nullptr; }
	}

	void c_frame_buffer::clear() {
        shared.context->ClearRenderTargetView(render_target, color_t<float>{ 0.f, 0.f, 0.f, 0.f }.channels.data());
        shared.context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void c_frame_buffer::set() {
        shared.context->OMSetRenderTargets(1, &render_target, depth_stencil_view);
        shared.context->RSSetState(rasterizer_state);
	}

    void c_frame_buffer::copy_from(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) {
        D3D11_TEXTURE2D_DESC desc{ };
        render_target_texture->GetDesc(&desc);

        if(another_frame_buffer->flags & e_frame_buffer_flags::msaa) {
            shared.context->ResolveSubresource(render_target_texture, 0, (ID3D11Resource*)another_frame_buffer->get_surface(), 0, desc.Format);
        } else {
            shared.context->CopyResource(render_target_texture, (ID3D11Resource*)another_frame_buffer->get_surface());
        }
    }
}