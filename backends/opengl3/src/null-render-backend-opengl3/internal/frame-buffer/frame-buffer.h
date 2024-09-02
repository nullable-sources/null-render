#pragma once
#include <null-render.h>

namespace ntl::render::opengl3 {
    class c_frame_buffer : public backend::i_frame_buffer {
    public: using i_frame_buffer::i_frame_buffer;
    public:
        std::uint32_t fbo{ }, fbo_attachment{ };

    public:
        void* get_surface() override { return (void*)fbo; }
        void* get_texture() override { return (void*)fbo_attachment; }

    public:
        void create() override;
        void destroy() override;

        void clear() override;
        void use() override;

        void copy_from(i_frame_buffer* another_frame_buffer) override;
        void blit_region_from(i_frame_buffer* another_frame_buffer, const vec2_t<int>& blit_offset, const rect_t<int>& region) override;
        void copy_in_texture(void* texture, const rect_t<int>& region) override;

    public:
        bool empty() const override { return fbo == 0; }
    };
}