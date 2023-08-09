#pragma once
#include "wrapper/shaders/pixel-shader.h"

namespace null::render::directx11 {
    namespace sources {
        static const std::vector<byte>& passthrough_color() {
            #include <shaders/compiled-objects/passthrough-color/compiled/passthrough-color.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_passthrough_color_shader_object : public c_pixel_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            create(sources::passthrough_color());
        }

        void on_destroy() override { destroy(); }
    } inline passthrough_color_shader_object{ };
}