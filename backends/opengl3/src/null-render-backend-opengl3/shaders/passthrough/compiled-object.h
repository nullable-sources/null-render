#pragma once
#include "../../wrapper/shader.h"

namespace null::render::opengl3 {
    class c_passthrough_fragment_shader_object : public c_fragment_shader {
    public:
        memory::resource_t get_source() override {
            return memory::resource_t("null-render:opengl:shaders:passthrough.frag", "null-render:opengl:resources");
        }
    } inline passthrough_fragment_shader_object{ };

    class c_passthrough_vertex_shader_object : public c_vertex_shader {
    public:
        memory::resource_t get_source() override {
            return memory::resource_t("null-render:opengl:shaders:passthrough.vert", "null-render:opengl:resources");
        }
    } inline passthrough_vertex_shader_object{ };
}