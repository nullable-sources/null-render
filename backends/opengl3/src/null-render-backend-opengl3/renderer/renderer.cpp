#include "../internal/shader/shader.h"
#include "../wrapper/opengl3.h"
#include "renderer.h"

namespace ntl::render::opengl3 {
    matrix4x4_t c_renderer::get_projection_matrix() const {
        return matrix4x4_t::projection_ortho(0.f, shared::viewport.x, shared::viewport.y, 0.f, -10000.f, 10000.f);
    }

    void c_renderer::set_texture(void* texture) {
        opengl::bind_texture(opengl::e_texture_2d, (std::uint32_t)texture);
    }

    void c_renderer::set_clip(const rect_t<float>& rect) {
        const vec2_t size = rect.size();
        opengl::scissor(rect.min.x, shared::viewport.y - rect.max.y, size.x, size.y);
    }

    void c_renderer::draw_geometry(backend::e_topology topology, size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) {
        opengl::draw_elements_base_vertex(topology == backend::e_topology::triangle_list ? opengl::e_triangles : opengl::e_triangle_strip, index_count, opengl::e_unsigned_int, (void*)(std::intptr_t)(index_offset * sizeof(std::uint32_t)), vertex_offset);
    }

    void* c_renderer::create_texture(const vec2_t<float>& size, void* data) {
        std::uint32_t texture{ };
        opengl::gen_textures(1, &texture);
        opengl::bind_texture(opengl::e_texture_2d, texture);
        opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_min_filter, opengl::e_linear);
        opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_mag_filter, opengl::e_linear);

        opengl::tex_image2d(opengl::e_texture_2d, 0, opengl::e_rgba, size.x, size.y, 0, opengl::e_rgba, opengl::e_unsigned_byte, data);
        opengl::bind_texture(opengl::e_texture_2d, 0);

        if(!texture) {
            sdk::logger(sdk::e_log_type::warning, "Failed to create texture.");
            return nullptr;
        }

        return (void*)texture;
    }

    void c_renderer::destroy_texture(void* texture) {
        if(!texture) {
            sdk::logger(sdk::e_log_type::warning, "It is impossible to destroy the texture because it is empty.");
            return;
        }

        opengl::delete_textures(1, (std::uint32_t*)&texture);
        texture = nullptr;
    }
}