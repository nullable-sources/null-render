#pragma once
#include "../color/compiled-object.h"

namespace ntl::render::opengl3 {
    class c_texture_shader_object : public c_fragment_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-render:opengl:shaders:texture.frag", "null-render:opengl:resources");
        }
    } inline texture_shader_object{ };
}