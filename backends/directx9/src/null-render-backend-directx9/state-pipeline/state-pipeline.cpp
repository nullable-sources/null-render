#include "state-pipeline.h"

namespace null::render::directx9 {
    void c_state_pipeline::setup_state() {
        D3DVIEWPORT9 _viewport{ 0, 0,
            (DWORD)shared::viewport.x,
            (DWORD)shared::viewport.y,
            0.0f, 1.0f
        };
        shared.device->SetViewport(&_viewport);

        shared.device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        shared.device->SetRenderState(D3DRS_LIGHTING, false);
        shared.device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        shared.device->SetRenderState(D3DRS_ZWRITEENABLE, false);
        shared.device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
        shared.device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        shared.device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
        shared.device->SetRenderState(D3DRS_FOGENABLE, false);
        shared.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        shared.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        shared.device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        shared.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        shared.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        shared.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
        shared.device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        shared.device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        shared.device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        shared.device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

        i_state_pipeline::setup_state();
    }

    void c_state_pipeline::save_state() {
        if(state_block) {
            if(auto result = state_block->Capture(); FAILED(result))
                utils::logger(utils::e_log_type::error, "cant capture state, error code {}.", result);
        }

        shared.device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, (DWORD*)&saved_rasterizer.multisample);
        shared.device->GetRenderState(D3DRS_SCISSORTESTENABLE, (DWORD*)&saved_rasterizer.scissor);

        shared.device->GetRenderState(D3DRS_ALPHABLENDENABLE, (DWORD*)&saved_blend.alpha_blend);
        shared.device->GetRenderState(D3DRS_SRCBLEND, (DWORD*)&saved_blend.src_blend);
        shared.device->GetRenderState(D3DRS_DESTBLEND, (DWORD*)&saved_blend.dst_blend);
        shared.device->GetRenderState(D3DRS_SRCBLENDALPHA, (DWORD*)&saved_blend.src_blend_alpha);
        shared.device->GetRenderState(D3DRS_DESTBLENDALPHA, (DWORD*)&saved_blend.dst_blend_alpha);
        shared.device->GetRenderState(D3DRS_COLORWRITEENABLE, (DWORD*)&saved_blend.color_write);

        shared.device->GetRenderTarget(0, &framebuffer);
        shared.device->GetDepthStencilSurface(&depth);
        shared.device->GetTexture(0, &texture);
        shared.device->GetPixelShader(&ps_shader);
        shared.device->GetVertexShader(&vs_shader);
        shared.device->GetVertexDeclaration(&vertex_declaration);
        shared.device->GetStreamSource(0, &vertex_buffer, &vertex_offset, &vertex_stride);
        shared.device->GetIndices(&index_buffer);
    }

    void c_state_pipeline::restore_state() {
        i_state_pipeline::restore_state();
        if(state_block) {
            if(auto result = state_block->Apply(); FAILED(result))
                utils::logger(utils::e_log_type::error, "cant apply state, error code {}.", result);
        }

        restore_framebuffer();
        restore_mesh();
        restore_shader();
        restore_texture();
        restore_rasterizer();
        restore_blend();

        if(framebuffer) { framebuffer->Release(); framebuffer = nullptr; }
        if(depth) { depth->Release(); depth = nullptr; }
        if(texture) { texture->Release(); texture = nullptr; }
        if(ps_shader) { ps_shader->Release(); ps_shader = nullptr; }
        if(vs_shader) { vs_shader->Release(); vs_shader = nullptr; }
        if(vertex_declaration) { vertex_declaration->Release(); vertex_declaration = nullptr; }
        if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
        if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
    }

    void c_state_pipeline::restore_framebuffer() {
        shared.device->SetRenderTarget(0, framebuffer);
        shared.device->SetDepthStencilSurface(depth);
    }

    void c_state_pipeline::restore_mesh() {
        shared.device->SetVertexDeclaration(vertex_declaration);
        shared.device->SetStreamSource(0, vertex_buffer, vertex_offset, vertex_stride);
        shared.device->SetIndices(index_buffer);
    }

    void c_state_pipeline::restore_shader() {
        shared.device->SetPixelShader(ps_shader);
        shared.device->SetVertexShader(vs_shader);
    }

    void c_state_pipeline::restore_texture() {
        shared.device->SetTexture(0, texture);
    }

    void c_state_pipeline::restore_rasterizer() {
        shared.device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, saved_rasterizer.multisample);
        shared.device->SetRenderState(D3DRS_SCISSORTESTENABLE, saved_rasterizer.scissor);
    }

    void c_state_pipeline::restore_blend() {
        shared.device->SetRenderState(D3DRS_ALPHABLENDENABLE, saved_blend.alpha_blend);
        shared.device->SetRenderState(D3DRS_SRCBLEND, saved_blend.src_blend);
        shared.device->SetRenderState(D3DRS_DESTBLEND, saved_blend.dst_blend);
        shared.device->SetRenderState(D3DRS_SRCBLENDALPHA, saved_blend.src_blend_alpha);
        shared.device->SetRenderState(D3DRS_DESTBLENDALPHA, saved_blend.dst_blend_alpha);
        shared.device->SetRenderState(D3DRS_COLORWRITEENABLE, saved_blend.color_write);
    }

    void c_state_pipeline::on_create() {
        if(state_block) return;
        if(auto result = shared.device->CreateStateBlock(D3DSBT_ALL, &state_block); FAILED(result))
            utils::logger(utils::e_log_type::warning, "cant create state block, return code {}.", result);
    }

    void c_state_pipeline::on_destroy() {
        if(state_block) { state_block->Release(); state_block = nullptr; }
    }
}