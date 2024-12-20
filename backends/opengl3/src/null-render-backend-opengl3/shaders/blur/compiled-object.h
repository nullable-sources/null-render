#pragma once
#include "null-render-backend-opengl3/shaders/passthrough/compiled-object.h"

namespace ntl::render::opengl3 {
    class c_blur_fragment_shader_object : public c_fragment_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-render:opengl:shaders:blur.frag", "null-render:opengl:resources");
        }
    } inline blur_fragment_shader_object{ };
}