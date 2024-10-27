#pragma once
#include "../../renderer/renderer.h"

namespace ntl::render::directx11 {
    class c_frame_buffer : public backend::i_frame_buffer {
    public: using i_frame_buffer::i_frame_buffer;
    public:
        ID3D11Texture2D* render_target_texture{ }, *depth_stencil_view_texture{ };
        ID3D11ShaderResourceView* shader_resource_view{ };
        ID3D11RenderTargetView* render_target{ };
        ID3D11DepthStencilView* depth_stencil_view{ };

        bool use_srgb{ false };

    public:
        void* get_surface() override { return render_target_texture; }
        void* get_texture() override { return shader_resource_view; }

    public:
        void create() override;
        void destroy() override;

    public:
        void clear() override;
        void use() override;

        void copy_from(i_frame_buffer* another_frame_buffer) override;
        void blit_region_from(i_frame_buffer* another_frame_buffer, const vec2_t<int>& blit_offset, const rect_t<int>& region) override;
        void copy_in_texture(void* texture, const rect_t<int>& region) override;

    public:
        bool empty() const override { return !render_target; }
    };
}