#pragma once
#include <backend/shaders/shader.h>
#include <wrapper/shader.h>

namespace null::render::opengl3 {
    class c_linear_gradient_shader_object : public c_fragment_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            create();
            compile(memory::resource_t{ "null-render:opengl:shaders:linear-gradient.frag", "null-render:opengl:resources" }.load());
        }

        void on_destroy() override { destroy(); }

    public:
        bool empty() const override { return shader == 0; }
    } inline linear_gradient_shader_object{ };
}