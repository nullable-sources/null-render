#include "frame-buffer.h"

namespace ntl::render::directx11 {
    void c_frame_buffer::set_from_external(void* external_resource) {
        if(render_target == external_resource) return;
        if(render_target) destroy();
        render_target = (ID3D11RenderTargetView*)external_resource;
        render_target->AddRef();

        ID3D11Resource* resource{ };
        render_target->GetResource(&resource);
        resource->QueryInterface<ID3D11Texture2D>(&render_target_texture);
        resource->Release();
    }

    void c_frame_buffer::create() {
        if(!empty()) return;
        if(type == backend::e_frame_buffer_type::external) return;

        //@note: creating texture for render target
        if(type == backend::e_frame_buffer_type::postprocessing) {
            D3D11_TEXTURE2D_DESC texture2d_desc{
                .Width{ (std::uint32_t)size.x },
                .Height{ (std::uint32_t)size.y },
                .MipLevels{ 1 },
                .ArraySize{ 1 },
                .Format{ use_srgb ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM },
                .SampleDesc{
                    .Count{ flags & backend::e_frame_buffer_flags::msaa ? render::shared::msaa_quality : 1 },
                    .Quality{ 0 }
                },
                .Usage{ D3D11_USAGE_DEFAULT },
                .BindFlags{ D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE },
                .CPUAccessFlags{ 0 }
            };

            if(auto result = shared.device->CreateTexture2D(&texture2d_desc, nullptr, &render_target_texture); FAILED(result)) {
                sdk::logger(sdk::e_log_type::error, "CreateTexture2D failed, return code {}.", result);
                return;
            }
        } else {
            if(auto result = shared.swap_chain->GetBuffer(0, IID_PPV_ARGS(&render_target_texture)); FAILED(result)) {
                sdk::logger(sdk::e_log_type::error, "GetBuffer failed, return code {}.", result);
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

        if(auto result = shared.device->CreateRenderTargetView(render_target_texture, &render_target_view_desc, &render_target); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "CreateRenderTargetView failed, return code {}.", result);
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
        if(auto result = shared.device->CreateShaderResourceView(render_target_texture, &shader_resource_view_desc, &shader_resource_view); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "CreateShaderResourceView failed, return code {}.", result);
            return;
        }

        if(type == backend::e_frame_buffer_type::backbuffer || (type == backend::e_frame_buffer_type::postprocessing && !(flags & backend::e_frame_buffer_flags::msaa))) {
            D3D11_TEXTURE2D_DESC depth_stencil_view_texture2d_desc{
                .Width{ render_target_texture_desc.Width },
                .Height{ render_target_texture_desc.Height },
                .MipLevels{ 1 },
                .ArraySize{ 1 },
                .Format{ DXGI_FORMAT_D24_UNORM_S8_UINT },
                .SampleDesc{ render_target_texture_desc.SampleDesc },
                .Usage{ D3D11_USAGE_DEFAULT },
                .BindFlags{ D3D11_BIND_DEPTH_STENCIL },
                .CPUAccessFlags{ 0 }
            };

            if(auto result = shared.device->CreateTexture2D(&depth_stencil_view_texture2d_desc, nullptr, &depth_stencil_view_texture); FAILED(result)) {
                sdk::logger(sdk::e_log_type::error, "CreateTexture2D failed, return code {}.", result);
                return;
            }

            D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{
                .Format{ DXGI_FORMAT_D24_UNORM_S8_UINT },
                .ViewDimension{ render_target_texture_desc.SampleDesc.Count > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D },
            };

            if(auto result = shared.device->CreateDepthStencilView(depth_stencil_view_texture, &depth_stencil_view_desc, &depth_stencil_view); FAILED(result)) {
                sdk::logger(sdk::e_log_type::error, "CreateDepthStencilView failed, return code {}.", result);
                return;
            }
        }
    }

    void c_frame_buffer::destroy() {
        if(render_target_texture) { render_target_texture->Release(); render_target_texture = nullptr; }
        if(depth_stencil_view_texture) { depth_stencil_view_texture->Release(); depth_stencil_view_texture = nullptr; }
        if(shader_resource_view) { shader_resource_view->Release(); shader_resource_view = nullptr; }
        if(render_target) { render_target->Release(); render_target = nullptr; }
        if(depth_stencil_view) { depth_stencil_view->Release(); depth_stencil_view = nullptr; }
    }

    void c_frame_buffer::clear() {
        shared.context->ClearRenderTargetView(render_target, color_t<float>(0.f).channels.data());
        if(depth_stencil_view)
            shared.context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void c_frame_buffer::use() {
        shared.context->OMSetRenderTargets(1, &render_target, depth_stencil_view ? depth_stencil_view : (ID3D11DepthStencilView*)backend::stencil_buffer->get_buffer());
    }

    void c_frame_buffer::copy_from(i_frame_buffer* another_frame_buffer) {
        if(another_frame_buffer->flags & backend::e_frame_buffer_flags::msaa) {
            D3D11_TEXTURE2D_DESC desc{ };
            render_target_texture->GetDesc(&desc);

            shared.context->ResolveSubresource(render_target_texture, 0, (ID3D11Resource*)another_frame_buffer->get_surface(), 0, desc.Format);
        } else {
            shared.context->CopyResource(render_target_texture, (ID3D11Resource*)another_frame_buffer->get_surface());
        }
    }

    void c_frame_buffer::blit_region_from(i_frame_buffer* another_frame_buffer, const vec2_t<int>& blit_offset, const rect_t<int>& region) {
        D3D11_BOX region_box{ };
        region_box.left = region.min.x;
        region_box.top = region.min.y;
        region_box.right = region.max.x;
        region_box.bottom = region.max.y;

        region_box.front = 0;
        region_box.back = 1;

        shared.context->CopySubresourceRegion(render_target_texture, 0, blit_offset.x, blit_offset.y, 0, (ID3D11Resource*)another_frame_buffer->get_surface(), 0, &region_box);
    }

    void c_frame_buffer::copy_in_texture(void* texture, const rect_t<int>& region) {
        D3D11_BOX region_box{ };
        region_box.left = region.min.x;
        region_box.top = region.min.y;
        region_box.right = region.max.x;
        region_box.bottom = region.max.y;

        region_box.front = 0;
        region_box.back = 1;

        ID3D11Resource* texture_resoruce{ };
        ((ID3D11View*)texture)->GetResource(&texture_resoruce);

        shared.context->CopySubresourceRegion(texture_resoruce, 0, region.min.x, region.min.y, 0, render_target_texture, 0, &region_box);
    }
}