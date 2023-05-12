#pragma once
#include <wrapper/pixel/pixel.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::directx9::shaders::compiled_objects {
    namespace sources {
        static const std::vector<byte>& linear_gradient() {
#include <shaders/compiled-objects/linear-gradient/compiled/linear-gradient.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_linear_gradient : public wrapper::c_pixel_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            compile(sources::linear_gradient().data());
        }
    } inline linear_gradient{ };
}