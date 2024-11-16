#include "renderer.h"

namespace ntl::render::directx11 {
    matrix4x4_t c_renderer::get_projection_matrix() const {
        return matrix4x4_t::projection_ortho(0.f, render::shared::viewport.x, render::shared::viewport.y, 0.f, -10000.f, 10000.f);
    }

    void c_renderer::set_texture(void* texture) {
        shared.context->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&texture);
    }

    void c_renderer::set_clip(const rect_t<float>& rect) {
        const D3D11_RECT clip(rect.min.x, rect.min.y, rect.max.x, rect.max.y);
        shared.context->RSSetScissorRects(1, &clip);
    }

    void c_renderer::draw_geometry(backend::e_topology topology, size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) {
        if(old_topology != topology) {
            shared.context->IASetPrimitiveTopology(topology == backend::e_topology::triangle_list ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
            old_topology = topology;
        }
        shared.context->DrawIndexed(index_count, index_offset, vertex_offset);
    }

    void* c_renderer::create_texture(const vec2_t<float>& size, void* data) {
        D3D11_TEXTURE2D_DESC texture_desc{
            .Width{ (std::uint32_t)size.x },
            .Height{ (std::uint32_t)size.y },
            .MipLevels{ 1 },
            .ArraySize{ 1 },
            .Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
            .SampleDesc{
                .Count{ 1 }
            },
            .Usage{ D3D11_USAGE_DEFAULT },
            .BindFlags{ D3D11_BIND_SHADER_RESOURCE },
            .CPUAccessFlags{ 0 }
        };

        ID3D11Texture2D* texture{ };
        D3D11_SUBRESOURCE_DATA subresource{
            .pSysMem{ (void*)data },
            .SysMemPitch{ texture_desc.Width * 4 },
            .SysMemSlicePitch{ 0 }
        };
        if(auto result = shared.device->CreateTexture2D(&texture_desc, data ? &subresource : nullptr, &texture); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant create texture2d, return code {}.", result);

        ID3D11ShaderResourceView* texture_view{ };
        D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{
            .Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
            .ViewDimension{ D3D11_SRV_DIMENSION_TEXTURE2D },
            .Texture2D{
                .MostDetailedMip{ 0 },
                .MipLevels{ texture_desc.MipLevels }
            }
        };
        if(auto result = shared.device->CreateShaderResourceView(texture, &shader_resource_view_desc, &texture_view); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant create shader resource view, return code {}.", result);

        if(auto result = texture->Release(); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant release texture, return code {}.", result);

        return texture_view;
    }

    void c_renderer::destroy_texture(void* texture) {
        if(!texture) return;

        if(auto result = ((ID3D11ShaderResourceView*)texture)->Release(); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant release texture, return code {}.", result);
        texture = nullptr;
    }

    void c_renderer::create_internal_objects() {
        if(!internal_objects.sampler) {
            D3D11_SAMPLER_DESC sampler_desc{
                .Filter{ D3D11_FILTER_MIN_MAG_MIP_LINEAR },
                .AddressU{ D3D11_TEXTURE_ADDRESS_CLAMP },
                .AddressV{ D3D11_TEXTURE_ADDRESS_CLAMP },
                .AddressW{ D3D11_TEXTURE_ADDRESS_WRAP },
                .MipLODBias{ 0.f },
                .ComparisonFunc{ D3D11_COMPARISON_ALWAYS },
                .MinLOD{ 0.f },
                .MaxLOD{ 0.f }
            };
            if(auto result = shared.device->CreateSamplerState(&sampler_desc, &internal_objects.sampler); FAILED(result))
                sdk::logger(sdk::e_log_type::error, "cant create sampler state, return code {}.", result);
        }
    }

    void c_renderer::destroy_internal_objects() {
        if(internal_objects.sampler) { internal_objects.sampler->Release(); internal_objects.sampler = nullptr; }
    }

    void c_renderer::on_setup_state() {
        shared.context->PSSetSamplers(0, 1, &internal_objects.sampler);
    }
}