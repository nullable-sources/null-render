#pragma once
#include "../../wrapper/shader.h"

namespace ntl::render::opengl3 {
    class c_color_fragment_shader_object : public c_fragment_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-render:opengl:shaders:color.frag", "null-render:opengl:resources");
        }
    } inline color_fragment_shader_object{ };

    class c_color_vertex_shader_object : public c_vertex_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-render:opengl:shaders:color.vert", "null-render:opengl:resources");
        }
    } inline color_vertex_shader_object{ };
}