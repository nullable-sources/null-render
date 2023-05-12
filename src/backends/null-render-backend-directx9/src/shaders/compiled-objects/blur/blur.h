#pragma once
#include <wrapper/pixel/pixel.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::directx9::shaders::compiled_objects {
    namespace sources {
        static const std::vector<byte>& blur() {
#include <shaders/compiled-objects/blur/compiled/blur.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_blur : public wrapper::c_pixel_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            compile(sources::blur().data());
        }
    } inline blur{ };
}