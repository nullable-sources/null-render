#pragma once
#include "wrapper/shaders/pixel-shader.h"

namespace null::render::directx11 {
    namespace sources {
        static const std::vector<byte>& passthrough_texture() {
            #include <shaders/compiled-objects/passthrough-texture/compiled/passthrough-texture.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_passthrough_texture_shader_object : public c_pixel_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            create(sources::passthrough_texture());
        }

        void on_destroy() override { destroy(); }
    } inline passthrough_texture_shader_object{ };
}