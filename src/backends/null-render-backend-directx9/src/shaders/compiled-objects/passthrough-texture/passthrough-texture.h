#pragma once
#include "wrapper/pixel/pixel.h"

namespace null::render::directx9 {
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
            compile(sources::passthrough_texture().data());
        }
    } inline passthrough_texture_shader_object{ };
}