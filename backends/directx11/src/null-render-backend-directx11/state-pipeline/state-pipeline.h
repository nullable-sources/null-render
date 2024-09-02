#pragma once
#include <null-render/backend/state-pipeline/state-pipeline.h>
#include "../renderer/renderer.h"

namespace ntl::render::directx11 {
    class c_state_pipeline : public backend::i_state_pipeline {
    public:
        struct {
            ID3D11RenderTargetView* render_target_view{ };
            ID3D11DepthStencilView* depth_stencil_view{ };
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
        } saved_state{ };

    public:
        void setup_state() override;
        void save_state() override;
        void restore_state() override;

    public:
        void restore_framebuffer() override;
        void restore_shader() override;
        void restore_mesh() override;
        void restore_texture() override;
        void restore_rasterizer() override;
        void restore_blend() override;
    };
}
