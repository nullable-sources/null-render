#include <null-renderer-directx11.h>
#include <shaders/vertex/vertex.h>
#include <shaders/pixel-without-sampler/pixel-without-sampler.h>
#include <shaders/pixel-sampler/pixel-sampler.h>

struct directx11_state_t {
    UINT scissor_rects_count{ }, viewports_count{ };
    D3D11_RECT scissor_rects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ };
    D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ };
    ID3D11RasterizerState* rasterizer_state{ };
    ID3D11BlendState* blend_state{ };
    FLOAT blend_factor[4]{ };
    UINT sample_mask{ };
    UINT stencil_ref{ };
    ID3D11DepthStencilState* depth_stencil_state{ };
    ID3D11ShaderResourceView* shader_resource{ };
    ID3D11SamplerState* sampler{ };
    ID3D11PixelShader* pixel_shader{ };
    ID3D11VertexShader* vertex_shader{ };
    ID3D11GeometryShader* geometry_shader{ };
    UINT pixel_shader_instances_count{ }, vertex_shader_instances_count{ }, geometry_shader_instances_count{ };
    ID3D11ClassInstance* pixel_shader_instances[256]{ }, *vertex_shader_instances[256]{ }, *geometry_shader_instances[256]{ };
    D3D11_PRIMITIVE_TOPOLOGY primitive_topology{ };
    ID3D11Buffer* index_buffer{ }, *vertex_buffer{ }, *vertex_shader_constant_buffer{ };
    UINT index_buffer_offset{ }, vertex_buffer_stride{ }, vertex_buffer_offset{ };
    DXGI_FORMAT index_buffer_format{ };
    ID3D11InputLayout* input_layout{ };

    void save(ID3D11DeviceContext* context) {
        scissor_rects_count = viewports_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
        context->RSGetScissorRects(&scissor_rects_count, scissor_rects);
        context->RSGetViewports(&viewports_count, viewports);
        context->RSGetState(&rasterizer_state);
        context->OMGetBlendState(&blend_state, blend_factor, &sample_mask);
        context->OMGetDepthStencilState(&depth_stencil_state, &stencil_ref);
        context->PSGetShaderResources(0, 1, &shader_resource);
        context->PSGetSamplers(0, 1, &sampler);
        pixel_shader_instances_count = vertex_shader_instances_count = geometry_shader_instances_count = 256;
        context->PSGetShader(&pixel_shader, pixel_shader_instances, &pixel_shader_instances_count);
        context->VSGetShader(&vertex_shader, vertex_shader_instances, &vertex_shader_instances_count);
        context->VSGetConstantBuffers(0, 1, &vertex_shader_constant_buffer);
        context->GSGetShader(&geometry_shader, geometry_shader_instances, &geometry_shader_instances_count);

        context->IAGetPrimitiveTopology(&primitive_topology);
        context->IAGetIndexBuffer(&index_buffer, &index_buffer_format, &index_buffer_offset);
        context->IAGetVertexBuffers(0, 1, &vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
        context->IAGetInputLayout(&input_layout);
    }

    void restore(ID3D11DeviceContext* context) {
        context->RSSetScissorRects(scissor_rects_count, scissor_rects);
        context->RSSetViewports(viewports_count, viewports);
        context->RSSetState(rasterizer_state); if(rasterizer_state) rasterizer_state->Release();
        context->OMSetBlendState(blend_state, blend_factor, sample_mask); if(blend_state) blend_state->Release();
        context->OMSetDepthStencilState(depth_stencil_state, stencil_ref); if(depth_stencil_state) depth_stencil_state->Release();
        context->PSSetShaderResources(0, 1, &shader_resource); if(shader_resource) shader_resource->Release();
        context->PSSetSamplers(0, 1, &sampler); if(sampler) sampler->Release();
        context->PSSetShader(pixel_shader, pixel_shader_instances, pixel_shader_instances_count); if(pixel_shader) pixel_shader->Release();
        for(const std::uint32_t& i : std::views::iota((std::uint32_t)0, pixel_shader_instances_count)) if(pixel_shader_instances[i]) pixel_shader_instances[i]->Release();
        context->VSSetShader(vertex_shader, vertex_shader_instances, vertex_shader_instances_count); if(vertex_shader) vertex_shader->Release();
        context->VSSetConstantBuffers(0, 1, &vertex_shader_constant_buffer); if(vertex_shader_constant_buffer) vertex_shader_constant_buffer->Release();
        context->GSSetShader(geometry_shader, geometry_shader_instances, geometry_shader_instances_count); if(geometry_shader) geometry_shader->Release();
        for(const std::uint32_t& i : std::views::iota((std::uint32_t)0, vertex_shader_instances_count)) if(vertex_shader_instances[i]) vertex_shader_instances[i]->Release();
        context->IASetPrimitiveTopology(primitive_topology);
        context->IASetIndexBuffer(index_buffer, index_buffer_format, index_buffer_offset); if(index_buffer) index_buffer->Release();
        context->IASetVertexBuffers(0, 1, &vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset); if(vertex_buffer) vertex_buffer->Release();
        context->IASetInputLayout(input_layout); if(input_layout) input_layout->Release();
    }
};

namespace null::renderer {
    void c_directx11::initialize() {
        IDXGIDevice* dxg_device{ };
        IDXGIAdapter* dxgi_adapter{ };
        IDXGIFactory* local_factory{ };

        if(device->QueryInterface(IID_PPV_ARGS(&dxg_device)) == S_OK) {
            if(dxg_device->GetParent(IID_PPV_ARGS(&dxgi_adapter)) == S_OK) {
                if(dxgi_adapter->GetParent(IID_PPV_ARGS(&local_factory)) == S_OK) {
                    factory = local_factory;
                }
            }
        }
        if(dxg_device) dxg_device->Release();
        if(dxgi_adapter) dxgi_adapter->Release();
        device->AddRef();
        context->AddRef();
    }

    void c_directx11::shutdown() {
        destroy_objects();
        if(factory) { factory->Release(); factory = nullptr; }
        if(device) { device->Release(); device = nullptr; }
        if(context) { context->Release(); context = nullptr; }
    }

    void c_directx11::render(const compiled_geometry_data_t& _compiled_geometry_data) {
        if(render::shared::viewport <= 0.f)
            return;

        static int vertex_buffer_size{ 5000 }, index_buffer_size{ 10000 };
        if(!vertex_buffer || vertex_buffer_size < _compiled_geometry_data.total_vtx_count) {
            if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
            vertex_buffer_size = _compiled_geometry_data.total_vtx_count + 5000;
            D3D11_BUFFER_DESC buffer_desc{
                .ByteWidth{ vertex_buffer_size * sizeof(vertex_t) },
                .Usage{ D3D11_USAGE_DYNAMIC },
                .BindFlags{ D3D11_BIND_VERTEX_BUFFER },
                .CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE },
                .MiscFlags{ 0 }
            };
            if(device->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer) < 0)
                throw std::runtime_error{ "cant create vertex buffer" };
        }

        if(!index_buffer || index_buffer_size < _compiled_geometry_data.total_idx_count) {
            if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
            index_buffer_size = _compiled_geometry_data.total_idx_count + 10000;
            D3D11_BUFFER_DESC buffer_desc{
                .ByteWidth{ index_buffer_size * sizeof(std::uint32_t) },
                .Usage{ D3D11_USAGE_DYNAMIC },
                .BindFlags{ D3D11_BIND_INDEX_BUFFER },
                .CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE },
                .MiscFlags{ 0 }
            };

            if(device->CreateBuffer(&buffer_desc, nullptr, &index_buffer) < 0)
                throw std::runtime_error{ "cant create index buffer" };
        }

        D3D11_MAPPED_SUBRESOURCE vtx_resource{ }, idx_resource{ };
        if(context->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK) throw std::runtime_error{ "cant map vertex buffer" };
        if(context->Map(index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK) throw std::runtime_error{ "cant map index buffer" };

        vertex_t* vtx_dst{ (vertex_t*)vtx_resource.pData };
        std::uint32_t* idx_dst{ (std::uint32_t*)idx_resource.pData };
        for(render::c_geometry_buffer* geometry_buffer : _compiled_geometry_data.geometry_buffers) {
            for(const render::vertex_t& vertex : geometry_buffer->vtx_buffer) {
                *vtx_dst = {
                    { vertex.pos.x, vertex.pos.y, 0.f },
                    (std::uint32_t)((vertex.color.a & 0xff) << 24) | ((vertex.color.b & 0xff) << 16) | ((vertex.color.g & 0xff) << 8) | (vertex.color.r & 0xff),
                    { vertex.uv.x, vertex.uv.y }
                };

                vtx_dst++;
            }
            memcpy(idx_dst, geometry_buffer->idx_buffer.data(), geometry_buffer->idx_buffer.size() * sizeof(std::uint32_t));
            idx_dst += geometry_buffer->idx_buffer.size();
        }
        context->Unmap(vertex_buffer, 0);
        context->Unmap(index_buffer, 0);

        directx11_state_t directx11_state{ };
        directx11_state.save(context);

        setup_state();

        for(int global_idx_offset{ }, global_vtx_offset{ }; render::c_geometry_buffer* geometry_buffer : _compiled_geometry_data.geometry_buffers) {
            for(render::c_geometry_buffer::cmd_t& cmd : geometry_buffer->cmd_buffer) {
                if(auto& callback{ cmd.callbacks.at<render::e_cmd_callbacks::on_draw>() }; !callback.empty() && callback.call(cmd)) {
                    setup_state();
                    continue;
                }

                const D3D11_RECT clip_rect{ (LONG)cmd.clip_rect.min.x, (LONG)cmd.clip_rect.min.y, (LONG)cmd.clip_rect.max.x, (LONG)cmd.clip_rect.max.y };
                context->RSSetScissorRects(1, &clip_rect);

                if(!cmd.texture) render::shaders::pixel_without_sampler::shader.set();
                else {
                    render::shaders::pixel_sampler::shader.set();
                    context->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&cmd.texture);
                }
                context->DrawIndexed(cmd.element_count, global_idx_offset, global_vtx_offset);
                global_idx_offset += cmd.element_count;
            }
            global_vtx_offset += geometry_buffer->vtx_buffer.size();
        }

        directx11_state.restore(context);
    }

    void c_directx11::setup_state() {
        D3D11_VIEWPORT viewport{ 0, 0,
            render::shared::viewport.x,
            render::shared::viewport.y,
            0, 1
        };
        context->RSSetViewports(1, &viewport);

        render::shaders::vertex::shader.setup_state();
        render::shaders::pixel_sampler::shader.setup_state();
        render::shaders::pixel_without_sampler::shader.setup_state();

        std::uint32_t stride{ sizeof(vertex_t) };
        std::uint32_t offset{ };
        context->IASetInputLayout(input_layout);
        context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
        context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        context->PSSetSamplers(0, 1, &font_sampler);
        context->GSSetShader(nullptr, nullptr, 0);
        context->HSSetShader(nullptr, nullptr, 0);
        context->DSSetShader(nullptr, nullptr, 0);
        context->CSSetShader(nullptr, nullptr, 0);

        const float blend_factor[4]{ 0.f, 0.f, 0.f, 0.f };
        context->OMSetBlendState(blend_state, blend_factor, 0xffffffff);
        context->OMSetDepthStencilState(depth_stencil_state, 0);
        context->RSSetState(rasterizer_state);
    }

    void c_directx11::create_objects() {
        if(!device) return;
        if(font_sampler) destroy_objects();

        render::shaders::vertex::shader.create();
        render::shaders::pixel_sampler::shader.create();
        render::shaders::pixel_without_sampler::shader.create();

        {
            D3D11_INPUT_ELEMENT_DESC local_layout[] = {
                        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)offsetof(vertex_t, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)offsetof(vertex_t, uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)offsetof(vertex_t, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            device->CreateInputLayout(local_layout, 3, render::shaders::sources::vertex::shader_data, sizeof(render::shaders::sources::vertex::shader_data), &input_layout);
        }


        {
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
            device->CreateBlendState(&blend_desc, &blend_state);
        }

        {
            D3D11_RASTERIZER_DESC rasterizer_desc{
                .FillMode{ D3D11_FILL_SOLID },
                .CullMode{ D3D11_CULL_NONE },
                .DepthClipEnable{ true },
                .ScissorEnable{ true }
            };
            device->CreateRasterizerState(&rasterizer_desc, &rasterizer_state);
        }

        {
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
            device->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state);
        }

        create_fonts_texture();
    }

    void c_directx11::destroy_objects() {
        if(!device) return;

        render::shaders::vertex::shader.destroy();
        render::shaders::pixel_sampler::shader.destroy();
        render::shaders::pixel_without_sampler::shader.destroy();

        if(font_sampler) { font_sampler->Release(); font_sampler = nullptr; }
        if(font_texture_view) { font_texture_view->Release(); font_texture_view = nullptr; render::atlas.texture.data = nullptr; }
        if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
        if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }

        if(blend_state) { blend_state->Release(); blend_state = nullptr; }
        if(depth_stencil_state) { depth_stencil_state->Release(); depth_stencil_state = nullptr; }
        if(rasterizer_state) { rasterizer_state->Release(); rasterizer_state = nullptr; }
        if(input_layout) { input_layout->Release(); input_layout = nullptr; }
    }

    void c_directx11::create_fonts_texture() {
        if(render::atlas.texture.pixels_alpha8.empty()) {
            if(render::atlas.configs.empty()) render::atlas.add_font_default();
            render::atlas.build();
        }

        render::atlas.texture.get_data_as_rgba32();

        {
            D3D11_TEXTURE2D_DESC texture_desc{
                .Width{ (std::uint32_t)render::atlas.texture.size.x },
                .Height{ (std::uint32_t)render::atlas.texture.size.y },
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
                .pSysMem{ (void*)render::atlas.texture.pixels_rgba32.data() },
                .SysMemPitch{ texture_desc.Width * 4 },
                .SysMemSlicePitch{ 0 }
            };
            device->CreateTexture2D(&texture_desc, &subresource, &texture);

            D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{
                .Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
                .ViewDimension{ D3D11_SRV_DIMENSION_TEXTURE2D },
                .Texture2D{
                    .MostDetailedMip{ 0 },
                    .MipLevels{ texture_desc.MipLevels }
                }
            };
            device->CreateShaderResourceView(texture, &shader_resource_view_desc, &font_texture_view);
            texture->Release();
        }

        render::atlas.texture.data = (void*)font_texture_view;

        {
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
            device->CreateSamplerState(&sampler_desc, &font_sampler);
        }
    }
}