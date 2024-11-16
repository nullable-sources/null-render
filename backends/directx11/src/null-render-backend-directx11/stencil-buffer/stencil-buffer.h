#pragma once
#include "../renderer/renderer.h"

namespace ntl::render::directx11 {
    class c_stencil_buffer : public backend::i_stencil_buffer {
    public: using i_stencil_buffer::i_stencil_buffer;
    public:
        ID3D11DepthStencilView* buffer{ };
        ID3D11Texture2D* texture{ };

    public:
        void* get_buffer() override { return (void*)buffer; }

    public:
        void create() override;
        void destroy() override;

        void clear() override;

    public:
        bool empty() const override { return !buffer; }
    };
}