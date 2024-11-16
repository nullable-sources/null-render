#pragma once
#include <null-render.h>

namespace ntl::render::opengl3 {
    class c_stencil_buffer : public backend::i_stencil_buffer {
    public: using i_stencil_buffer::i_stencil_buffer;
    public:
        std::uint32_t buffer{ };

    public:
        void* get_buffer() override { return (void*)buffer; }

    public:
        void create() override;
        void destroy() override;

        void clear() override;

    public:
        bool empty() const override { return buffer == 0; }
    };
}