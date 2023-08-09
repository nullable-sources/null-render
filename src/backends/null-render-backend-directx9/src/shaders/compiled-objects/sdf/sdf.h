#pragma once
#include "wrapper/pixel/pixel.h"

namespace null::render::directx9 {
    namespace sources {
        static const std::vector<byte>& sdf() {
            #include <shaders/compiled-objects/sdf/compiled/sdf.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_sdf_shader_object : public c_pixel_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            compile(sources::sdf().data());
        }
    } inline sdf_shader_object{ };
}