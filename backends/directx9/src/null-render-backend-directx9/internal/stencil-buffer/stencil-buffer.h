#pragma once
#include "../../renderer/renderer.h"

namespace ntl::render::directx9 {
    class c_stencil_buffer : public backend::i_stencil_buffer {
    public: using i_stencil_buffer::i_stencil_buffer;
    public:
        IDirect3DSurface9* buffer{ };
        int stencil_ref{ -1 };

    public:
        void* get_buffer() override { return (void*)buffer; }

    public:
        void create() override;
        void destroy() override;

        void clear() override;

        void set_test(bool test) override;
        void set_operation(backend::e_stencil_operation operation) override;

    public:
        bool empty() const override { return buffer == nullptr; }
    };
}