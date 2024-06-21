#pragma once
#include "../../wrapper/fields.h"

namespace null::render::opengl3 {
    class c_mesh : public backend::c_mesh {
    public:
        c_vertex_element<backend::vertex_t, 2, opengl::e_float, false, offsetof(backend::vertex_t, pos)> position{ };
        c_vertex_element<backend::vertex_t, 4, opengl::e_int, false, offsetof(backend::vertex_t, color)> color{ };
        c_vertex_element<backend::vertex_t, 2, opengl::e_float, false, offsetof(backend::vertex_t, uv)> uv{ };
        std::uint32_t vao{ }, vbo{ }, ibo{ };

    public:
        void create() override;
        void destroy() override;

        void compile() override;

        void use() override;
    };
}