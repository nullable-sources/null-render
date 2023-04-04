#include <null-renderer-directx11.h>
#include <shaders/passthrough-color/passthrough-color.h>
#include <shaders/passthrough-texture/passthrough-texture.h>
#include <shaders/quad-gradient/quad-gradient.h>
#include <shaders/sdf/sdf.h>

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

namespace null::render {
    void c_directx11::set_texture(void* texture) {
        context->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&texture);
    }

    void c_directx11::set_clip(const rect_t<float>& rect) {
        const D3D11_RECT clip{ (LONG)rect.min.x, (LONG)rect.min.y, (LONG)rect.max.x, (LONG)rect.max.y };
        context->RSSetScissorRects(1, &clip);
    }

    void c_directx11::draw_geometry(const size_t& vertex_count, const size_t& index_count, const size_t& vertex_offset, const size_t& index_offset) {
        context->DrawIndexed(index_count, index_offset, vertex_offset);
    }

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

        impl::shaders::passthrough_color = std::make_unique<renderer::shaders::c_passthrough_color>();
        impl::shaders::passthrough_texture = std::make_unique<renderer::shaders::c_passthrough_texture>();

        impl::shaders::quad_gradient = std::make_unique<renderer::shaders::c_quad_gradient>();
        impl::shaders::sdf = std::make_unique<renderer::shaders::c_sdf>();
    }

    void c_directx11::shutdown() {
        destroy_objects();
        if(factory) { factory->Release(); factory = nullptr; }
        if(device) { device->Release(); device = nullptr; }
        if(context) { context->Release(); context = nullptr; }
    }

    void c_directx11::begin_render() {
        static int vertex_buffer_size{ 5000 }, index_buffer_size{ 10000 };
        if(!vertex_buffer || vertex_buffer_size < geometry_buffer.vertex_buffer.size()) {
            if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
            vertex_buffer_size = geometry_buffer.vertex_buffer.size() + 5000;
            D3D11_BUFFER_DESC buffer_desc{
                .ByteWidth{ vertex_buffer_size * sizeof(impl::vertex_t) },
                .Usage{ D3D11_USAGE_DYNAMIC },
                .BindFlags{ D3D11_BIND_VERTEX_BUFFER },
                .CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE },
                .MiscFlags{ 0 }
            };
            if(auto result{ device->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer) }; FAILED(result)) {
                utils::logger.log(utils::e_log_type::error, "cant create vertex buffer, return code {}.", result);
                return;
            }
        }

        if(!index_buffer || index_buffer_size < geometry_buffer.index_buffer.size()) {
            if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
            index_buffer_size = geometry_buffer.index_buffer.size() + 10000;
            D3D11_BUFFER_DESC buffer_desc{
                .ByteWidth{ index_buffer_size * sizeof(impl::index_t) },
                .Usage{ D3D11_USAGE_DYNAMIC },
                .BindFlags{ D3D11_BIND_INDEX_BUFFER },
                .CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE },
                .MiscFlags{ 0 }
            };

            if(auto result{ device->CreateBuffer(&buffer_desc, nullptr, &index_buffer) }; FAILED(result)) {
                utils::logger.log(utils::e_log_type::error, "cant create index buffer, return code {}.", result);
                return;
            }
        }

        D3D11_MAPPED_SUBRESOURCE vertex_buffer_subresource{ }, index_buffer_subresource{ };
        if(context->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertex_buffer_subresource) != S_OK) throw std::runtime_error{ "cant map vertex buffer" };
        if(context->Map(index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &index_buffer_subresource) != S_OK) throw std::runtime_error{ "cant map index buffer" };

        std::ranges::move(geometry_buffer.vertex_buffer, (impl::vertex_t*)vertex_buffer_subresource.pData);
        std::ranges::move(geometry_buffer.index_buffer, (impl::index_t*)index_buffer_subresource.pData);

        context->Unmap(vertex_buffer, 0);
        context->Unmap(index_buffer, 0);

        directx11_state_t directx11_state{ };
        directx11_state.save(context);

        setup_state();

        impl::shaders::passthrough_color->use();

        background.handle();
        background.clear();

        directx11_state.restore(context);
    }

    void c_directx11::end_render() {
        geometry_buffer.clear();
    }

    void c_directx11::setup_state() {
        D3D11_VIEWPORT viewport{ 0, 0,
            render::shared::viewport.x,
            render::shared::viewport.y,
            0, 1
        };
        context->RSSetViewports(1, &viewport);

        set_clip({ { 0 }, render::shared::viewport });
        set_matrix(matrix4x4_t::project_ortho(0.f, render::shared::viewport.x, render::shared::viewport.y, 0.f, -10000.f, 10000.f));
        impl::shaders::event_dispatcher.setup_state();

        std::uint32_t stride{ sizeof(impl::vertex_t) };
        std::uint32_t offset{ };
        context->IASetInputLayout(input_layout);
        context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
        context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        context->PSSetSamplers(0, 1, &sampler_state);
        context->GSSetShader(nullptr, nullptr, 0);
        context->HSSetShader(nullptr, nullptr, 0);
        context->DSSetShader(nullptr, nullptr, 0);
        context->CSSetShader(nullptr, nullptr, 0);

        constexpr float blend_factor[4]{ 0.f, 0.f, 0.f, 0.f };
        context->OMSetBlendState(blend_state, blend_factor, 0xffffffff);
        context->OMSetDepthStencilState(depth_stencil_state, 0);
        context->RSSetState(rasterizer_state);
    }

    void c_directx11::create_objects() {
        if(!device) return;
        if(sampler_state) destroy_objects();

        impl::shaders::event_dispatcher.create();

        {
            D3D11_INPUT_ELEMENT_DESC local_layout[] = {
                        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,      0, (UINT)offsetof(impl::vertex_t, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,      0, (UINT)offsetof(impl::vertex_t, uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_UINT, 0, (UINT)offsetof(impl::vertex_t, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            if(auto result{ device->CreateInputLayout(local_layout, 3, renderer::shaders::sources::passthrough_texture_vertex().data(), renderer::shaders::sources::passthrough_texture_vertex().size(), &input_layout) }; FAILED(result))
                utils::logger.log(utils::e_log_type::error, "cant create vertex input layout, return code {}.", result);
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
            if(auto result{ device->CreateBlendState(&blend_desc, &blend_state) }; FAILED(result))
                utils::logger.log(utils::e_log_type::error, "cant create blend state, return code {}.", result);
        }

        {
            D3D11_RASTERIZER_DESC rasterizer_desc{
                .FillMode{ D3D11_FILL_SOLID },
                .CullMode{ D3D11_CULL_NONE },
                .DepthClipEnable{ true },
                .ScissorEnable{ true }
            };
            if(auto result{ device->CreateRasterizerState(&rasterizer_desc, &rasterizer_state) }; FAILED(result))
                utils::logger.log(utils::e_log_type::error, "cant create rasterizer state, return code {}.", result);
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
            if(auto result{ device->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state) }; FAILED(result))
                utils::logger.log(utils::e_log_type::error, "cant create depth stencil state, return code {}.", result);
        }

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
            if(auto result{ device->CreateSamplerState(&sampler_desc, &sampler_state) }; FAILED(result))
                utils::logger.log(utils::e_log_type::error, "cant create sampler state, return code {}.", result);
        }

        create_atlases();
    }

    void c_directx11::destroy_objects() {
        if(!device) return;

        impl::shaders::event_dispatcher.destroy();

        destroy_atlases();

        if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
        if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }

        if(sampler_state) { sampler_state->Release(); sampler_state = nullptr; }
        if(blend_state) { blend_state->Release(); blend_state = nullptr; }
        if(depth_stencil_state) { depth_stencil_state->Release(); depth_stencil_state = nullptr; }
        if(rasterizer_state) { rasterizer_state->Release(); rasterizer_state = nullptr; }
        if(input_layout) { input_layout->Release(); input_layout = nullptr; }
    }

    void* c_directx11::create_texture(const vec2_t<float>& size, void* data) {
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
        if(auto result{ device->CreateTexture2D(&texture_desc, &subresource, &texture) }; FAILED(result))
            utils::logger.log(utils::e_log_type::error, "cant create texture2d, return code {}.", result);

        ID3D11ShaderResourceView* texture_view{ };
        D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{
            .Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
            .ViewDimension{ D3D11_SRV_DIMENSION_TEXTURE2D },
            .Texture2D{
                .MostDetailedMip{ 0 },
                .MipLevels{ texture_desc.MipLevels }
            }
        };
        if(auto result{ device->CreateShaderResourceView(texture, &shader_resource_view_desc, &texture_view) }; FAILED(result))
            utils::logger.log(utils::e_log_type::error, "cant create shader resource view, return code {}.", result);

        if(auto result{ texture->Release() }; FAILED(result))
            utils::logger.log(utils::e_log_type::warning, "cant release texture, return code {}.", result);

        return texture_view;
    }

    void c_directx11::destroy_texture(void* texture) {
        if(!texture) {
            utils::logger.log(utils::e_log_type::warning, "it is impossible to destroy the texture because it is empty.");
            return;
        }

        if(auto result{ ((ID3D11ShaderResourceView*)texture)->Release() }; FAILED(result))
            utils::logger.log(utils::e_log_type::warning, "cant release texture, return code {}.", result);
        texture = nullptr;
    }
}