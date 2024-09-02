#pragma once
#pragma comment (lib, "d3d11.lib")
#include <d3d11.h>

#ifdef NULL_RENDER_DX11_ALLOW_FEATURE_11_1
#include <d3d11_1.h>
#endif

namespace ntl::render::directx11 {
#ifdef NULL_RENDER_DX11_ALLOW_FEATURE_11_1
    using dx_device_t = ID3D11Device1;
    using dx_device_context_t = ID3D11DeviceContext1;
    using dx_rasterizer_state_t = ID3D11RasterizerState1;
    using dx_rasterizer_state_desc_t = D3D11_RASTERIZER_DESC1;
#else
    using dx_device_t = ID3D11Device;
    using dx_device_context_t = ID3D11DeviceContext;
    using dx_rasterizer_state_t = ID3D11RasterizerState;
    using dx_rasterizer_state_desc_t = D3D11_RASTERIZER_DESC;
#endif

    struct dx_blend_factor_desc : public D3D11_BLEND_DESC {
    public:
        float blend_factor{ };

    public:
        dx_blend_factor_desc() { }
        dx_blend_factor_desc(const D3D11_BLEND_DESC& desc) : D3D11_BLEND_DESC(desc) { }
    };
}

#include <null-render.h>

namespace ntl::render::directx11 {
    struct shared_t {
    public:
        dx_device_t* device{ };
        dx_device_context_t* context{ };
        IDXGISwapChain* swap_chain{ };
    } inline shared{ };

    class c_renderer : public backend::i_renderer, public backend::i_renderer_event_listener {
    public:
        backend::e_topology old_topology{ };

        struct {
            ID3D11SamplerState* sampler{ };
            ID3D11DepthStencilState* depth_stencil{ };
        } internal_objects{ };

    public:
        c_renderer() {
            backend::renderer_event_dispatcher.attach_listener(backend::e_renderer_event_type::setup_state, this);
        }

        ~c_renderer() {
            backend::renderer_event_dispatcher.detach_listener(backend::e_renderer_event_type::setup_state, this);
        }

    public:
        constexpr virtual bool framebuffer_uvs_flipped() override { return false; }

    public:
        matrix4x4_t get_projection_matrix() const override;

        void set_texture(void* texture) override;
        void set_clip(const rect_t<float>& rect) override;
        void draw_geometry(backend::e_topology topology, size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) override;

        void* create_texture(const vec2_t<float>& size, void* data) override;
        void destroy_texture(void* texture) override;

    private:
        void create_internal_objects() override;
        void destroy_internal_objects() override;

    private:
        void on_setup_state() override;

    public:
        bool empty() const override { return !shared.device; }
    };
}