#pragma once
#include <wrapper/shaders/pixel-shader/pixel-shader.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::directx11::shaders::compiled_objects {
    namespace sources {
        static const std::vector<byte>& passthrough_texture() {
            #include <shaders/compiled-objects/passthrough-texture/compiled/passthrough-texture.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_passthrough_texture : public wrapper::c_pixel_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            create(sources::passthrough_texture());
        }

        void on_destroy() override { destroy(); }
    } inline passthrough_texture{ };
}