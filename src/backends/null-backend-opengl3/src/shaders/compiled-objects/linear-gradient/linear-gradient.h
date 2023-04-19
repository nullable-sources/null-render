#pragma once
#include <wrapper/shader/shader.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::opengl3::shaders::compiled_objects {
    class c_linear_gradient : public wrapper::c_fragment_shader, public backend::shaders::i_compiled_object {
    public:
        void on_create() override {
            if(!empty()) return;
            create();
            compile(memory::resource_t{ "null-render:opengl:shaders:linear-gradient.frag", "null-render:opengl:resources" }.load());
        }

        void on_destroy() override { destroy(); }

    public:
        bool empty() const override { return shader == 0; }
    } inline linear_gradient{ };
}