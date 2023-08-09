#pragma once
#include "wrapper/shader.h"

namespace null::render::opengl3 {
    class c_passthrough_texture_shader_object : public c_fragment_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            create();
            compile(memory::resource_t{ "null-render:opengl:shaders:passthrough-texture.frag", "null-render:opengl:resources" }.load());
        }

        void on_destroy() override { destroy(); }

    public:
        bool empty() const override { return shader == 0; }
    } inline passthrough_texture_shader_object{ };
}