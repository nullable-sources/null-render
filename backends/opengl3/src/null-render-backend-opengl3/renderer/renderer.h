#pragma once
#include <null-render.h>

namespace ntl::render::opengl3 {
    class c_renderer : public backend::i_renderer {
    public:
        constexpr bool framebuffer_uvs_flipped() override { return true; }

    public:
        matrix4x4_t get_projection_matrix() const override;

        void set_texture(void* texture) override;
        void set_clip(const rect_t<float>& rect) override;
        void draw_geometry(backend::e_topology topology, size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) override;

        void* create_texture(const vec2_t<float>& size, void* data) override;
        void destroy_texture(void* texture) override;

    public:
        bool empty() const override { return false; }
    };
}