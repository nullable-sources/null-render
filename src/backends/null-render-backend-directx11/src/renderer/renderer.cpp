#include "renderer/renderer.h"

namespace null::render::directx11 {
    matrix4x4_t c_renderer::get_projection_matrix() const {
        return matrix4x4_t::projection_ortho(0.f, render::shared::viewport.x, render::shared::viewport.y, 0.f, -10000.f, 10000.f);
    }

    void c_renderer::set_texture(void* texture) {
        shared.context->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&texture);
    }

    void c_renderer::set_clip(const rect_t<float>& rect) {
        const D3D11_RECT clip{ (LONG)rect.min.x, (LONG)rect.min.y, (LONG)rect.max.x, (LONG)rect.max.y };
        shared.context->RSSetScissorRects(1, &clip);
    }

    void c_renderer::draw_geometry(size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) {
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
        if(auto result{ shared.device->CreateTexture2D(&texture_desc, &subresource, &texture) }; FAILED(result))
            utils::logger(utils::e_log_type::error, "cant create texture2d, return code {}.", result);

        ID3D11ShaderResourceView* texture_view{ };
        D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{
            .Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
            .ViewDimension{ D3D11_SRV_DIMENSION_TEXTURE2D },
            .Texture2D{
                .MostDetailedMip{ 0 },
                .MipLevels{ texture_desc.MipLevels }
            }
        };
        if(auto result{ shared.device->CreateShaderResourceView(texture, &shader_resource_view_desc, &texture_view) }; FAILED(result))
            utils::logger(utils::e_log_type::error, "cant create shader resource view, return code {}.", result);

        if(auto result{ texture->Release() }; FAILED(result))
            utils::logger(utils::e_log_type::warning, "cant release texture, return code {}.", result);

        return texture_view;
    }

    void c_renderer::destroy_texture(void* texture) {
        if(!texture) {
            utils::logger(utils::e_log_type::warning, "it is impossible to destroy the texture because it is empty.");
            return;
        }

        if(auto result{ ((ID3D11ShaderResourceView*)texture)->Release() }; FAILED(result))
            utils::logger(utils::e_log_type::warning, "cant release texture, return code {}.", result);
        texture = nullptr;
    }

    void c_renderer::setup_state() {
        D3D11_VIEWPORT viewport{ 0, 0,
            render::shared::viewport.x,
            render::shared::viewport.y,
            0, 1
        };
        shared.context->RSSetViewports(1, &viewport);

        set_clip({ { 0 }, render::shared::viewport });
        set_matrix(get_projection_matrix());
        backend::object_event_dispatcher.setup_state();

        backend::mesh->set();

        shared.context->PSSetSamplers(0, 1, &internal_objects.sampler);

        constexpr float blend_factor[4]{ 0.f, 0.f, 0.f, 0.f };
        shared.context->OMSetBlendState(internal_objects.blend, blend_factor, 0xffffffff);
        shared.context->OMSetDepthStencilState(internal_objects.depth_stencil, 0);
    }

    void c_renderer::save_state() {
        saved_state.scissor_rects_count = saved_state.viewports_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
        shared.context->RSGetScissorRects(&saved_state.scissor_rects_count, saved_state.scissor_rects);
        shared.context->RSGetViewports(&saved_state.viewports_count, saved_state.viewports);
        shared.context->RSGetState(&saved_state.rasterizer_state);
        shared.context->OMGetBlendState(&saved_state.blend_state, saved_state.blend_factor, &saved_state.sample_mask);
        shared.context->OMGetDepthStencilState(&saved_state.depth_stencil_state, &saved_state.stencil_ref);
        shared.context->PSGetShaderResources(0, 1, &saved_state.shader_resource);
        shared.context->PSGetSamplers(0, 1, &saved_state.sampler);
        saved_state.pixel_shader_instances_count = saved_state.vertex_shader_instances_count = saved_state.geometry_shader_instances_count = 256;
        shared.context->PSGetShader(&saved_state.pixel_shader, saved_state.pixel_shader_instances, &saved_state.pixel_shader_instances_count);
        shared.context->VSGetShader(&saved_state.vertex_shader, saved_state.vertex_shader_instances, &saved_state.vertex_shader_instances_count);
        shared.context->VSGetConstantBuffers(0, 1, &saved_state.vertex_shader_constant_buffer);
        shared.context->GSGetShader(&saved_state.geometry_shader, saved_state.geometry_shader_instances, &saved_state.geometry_shader_instances_count);

        shared.context->IAGetPrimitiveTopology(&saved_state.primitive_topology);
        shared.context->IAGetIndexBuffer(&saved_state.index_buffer, &saved_state.index_buffer_format, &saved_state.index_buffer_offset);
        shared.context->IAGetVertexBuffers(0, 1, &saved_state.vertex_buffer, &saved_state.vertex_buffer_stride, &saved_state.vertex_buffer_offset);
        shared.context->IAGetInputLayout(&saved_state.input_layout);
    }

    void c_renderer::restore_state() {
        shared.context->RSSetScissorRects(saved_state.scissor_rects_count, saved_state.scissor_rects);
        shared.context->RSSetViewports(saved_state.viewports_count, saved_state.viewports);
        shared.context->RSSetState(saved_state.rasterizer_state); if(saved_state.rasterizer_state) saved_state.rasterizer_state->Release();
        shared.context->OMSetBlendState(saved_state.blend_state, saved_state.blend_factor, saved_state.sample_mask); if(saved_state.blend_state) saved_state.blend_state->Release();
        shared.context->OMSetDepthStencilState(saved_state.depth_stencil_state, saved_state.stencil_ref); if(saved_state.depth_stencil_state) saved_state.depth_stencil_state->Release();
        shared.context->PSSetShaderResources(0, 1, &saved_state.shader_resource); if(saved_state.shader_resource) saved_state.shader_resource->Release();
        shared.context->PSSetSamplers(0, 1, &saved_state.sampler); if(saved_state.sampler) saved_state.sampler->Release();
        shared.context->PSSetShader(saved_state.pixel_shader, saved_state.pixel_shader_instances, saved_state.pixel_shader_instances_count); if(saved_state.pixel_shader) saved_state.pixel_shader->Release();
        for(std::uint32_t i : std::views::iota(0u, saved_state.pixel_shader_instances_count)) if(saved_state.pixel_shader_instances[i]) saved_state.pixel_shader_instances[i]->Release();
        shared.context->VSSetShader(saved_state.vertex_shader, saved_state.vertex_shader_instances, saved_state.vertex_shader_instances_count); if(saved_state.vertex_shader) saved_state.vertex_shader->Release();
        shared.context->VSSetConstantBuffers(0, 1, &saved_state.vertex_shader_constant_buffer); if(saved_state.vertex_shader_constant_buffer) saved_state.vertex_shader_constant_buffer->Release();
        shared.context->GSSetShader(saved_state.geometry_shader, saved_state.geometry_shader_instances, saved_state.geometry_shader_instances_count); if(saved_state.geometry_shader) saved_state.geometry_shader->Release();
        for(std::uint32_t i : std::views::iota(0u, saved_state.vertex_shader_instances_count)) if(saved_state.vertex_shader_instances[i]) saved_state.vertex_shader_instances[i]->Release();
        shared.context->IASetPrimitiveTopology(saved_state.primitive_topology);
        shared.context->IASetIndexBuffer(saved_state.index_buffer, saved_state.index_buffer_format, saved_state.index_buffer_offset); if(saved_state.index_buffer) saved_state.index_buffer->Release();
        shared.context->IASetVertexBuffers(0, 1, &saved_state.vertex_buffer, &saved_state.vertex_buffer_stride, &saved_state.vertex_buffer_offset); if(saved_state.vertex_buffer) saved_state.vertex_buffer->Release();
        shared.context->IASetInputLayout(saved_state.input_layout); if(saved_state.input_layout) saved_state.input_layout->Release();
    }

    void c_renderer::create_internal_objects() {
        if(!internal_objects.blend) {
            D3D11_BLEND_DESC blend_desc{
                .AlphaToCoverageEnable{ false },
                .RenderTarget{
                    {
                        .BlendEnable{ true },
                        .SrcBlend{ D3D11_BLEND_SRC_ALPHA  },
                        .DestBlend{ D3D11_BLEND_INV_SRC_ALPHA },
                        .BlendOp{ D3D11_BLEND_OP_ADD },
                        .SrcBlendAlpha{ D3D11_BLEND_ONE },
                        .DestBlendAlpha{ D3D11_BLEND_INV_SRC_ALPHA },
                        .BlendOpAlpha{ D3D11_BLEND_OP_ADD },
                        .RenderTargetWriteMask{ D3D11_COLOR_WRITE_ENABLE_ALL }
                    }
                }
            };
            if(auto result{ shared.device->CreateBlendState(&blend_desc, &internal_objects.blend) }; FAILED(result))
                utils::logger(utils::e_log_type::error, "cant create blend state, return code {}.", result);
        }

        if(!internal_objects.depth_stencil) {
            D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{
                .DepthEnable{ false },
                .DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
                .DepthFunc = D3D11_COMPARISON_ALWAYS,
                .StencilEnable = false,
                .FrontFace{
                    .StencilFailOp{ D3D11_STENCIL_OP_KEEP },
                    .StencilDepthFailOp{ D3D11_STENCIL_OP_KEEP },
                    .StencilPassOp{ D3D11_STENCIL_OP_KEEP },
                    .StencilFunc{ D3D11_COMPARISON_ALWAYS }
                },
                .BackFace {
                    .StencilFailOp{ D3D11_STENCIL_OP_KEEP },
                    .StencilDepthFailOp{ D3D11_STENCIL_OP_KEEP },
                    .StencilPassOp{ D3D11_STENCIL_OP_KEEP },
                    .StencilFunc{ D3D11_COMPARISON_ALWAYS }
                }
            };
            if(auto result{ shared.device->CreateDepthStencilState(&depth_stencil_desc, &internal_objects.depth_stencil) }; FAILED(result))
                utils::logger(utils::e_log_type::error, "cant create depth stencil state, return code {}.", result);
        }

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
            if(auto result{ shared.device->CreateSamplerState(&sampler_desc, &internal_objects.sampler) }; FAILED(result))
                utils::logger(utils::e_log_type::error, "cant create sampler state, return code {}.", result);
        }
    }

    void c_renderer::destroy_internal_objects() {
        if(internal_objects.sampler) { internal_objects.sampler->Release(); internal_objects.sampler = nullptr; }
        if(internal_objects.blend) { internal_objects.blend->Release(); internal_objects.blend = nullptr; }
        if(internal_objects.depth_stencil) { internal_objects.depth_stencil->Release(); internal_objects.depth_stencil = nullptr; }
    }
}