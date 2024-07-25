#pragma once
#include <null-render/backend/state-pipeline/state-pipeline.h>
#include "../renderer/renderer.h"

namespace null::render::directx9 {
    class c_state_pipeline : public backend::i_state_pipeline, public backend::i_renderer_event_listener {
    public:
        IDirect3DStateBlock9* state_block{ };

        IDirect3DSurface9* framebuffer{ }, *depth{ };
        IDirect3DBaseTexture9* texture{ };
        IDirect3DPixelShader9* ps_shader{ };
        IDirect3DVertexShader9* vs_shader{ };

        IDirect3DVertexDeclaration9* vertex_declaration{ };
        IDirect3DVertexBuffer9* vertex_buffer{ };
        IDirect3DIndexBuffer9* index_buffer{ };
        std::uint32_t vertex_offset{ }, vertex_stride{ };

        struct saved_rasterizer_t {
        public:
            std::uint32_t multisample{ }, scissor{ };
        } saved_rasterizer{ };

        struct saved_blend_t {
        public:
            std::uint32_t alpha_blend{ };
            std::uint32_t src_blend{ }, src_blend_alpha{ };
            std::uint32_t dst_blend{ }, dst_blend_alpha{ };
            std::uint32_t color_write{ };
            std::uint32_t blend_factor{ };
        } saved_blend{ };
        
    public:
        c_state_pipeline() {
            backend::renderer_event_dispatcher.attach_listener(backend::e_renderer_event_type::create, this);
            backend::renderer_event_dispatcher.attach_listener(backend::e_renderer_event_type::destroy, this);
        }

        ~c_state_pipeline() {
            backend::renderer_event_dispatcher.detach_listener(backend::e_renderer_event_type::create, this);
            backend::renderer_event_dispatcher.detach_listener(backend::e_renderer_event_type::destroy, this);
        }

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

    private:
        void on_create() override;
        void on_destroy() override;
    };
}
