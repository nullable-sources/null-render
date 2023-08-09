#pragma once
#include "wrapper/pixel/pixel.h"

namespace null::render::directx9 {
    namespace sources {
        static const std::vector<byte>& quad_gradient() {
            #include <shaders/compiled-objects/quad-gradient/compiled/quad-gradient.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_quad_gradient_shader_object : public c_pixel_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            compile(sources::quad_gradient().data());
        }
    } inline quad_gradient_shader_object{ };
}