#pragma once
#include "../color/compiled-object.h"

namespace null::render::directx11 {
    namespace sources {
        static const std::vector<byte>& texture() {
#include "compiled/texture.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_texture_shader_object : public c_pixel_shader {
    public:
        void create() override {
            if(!empty()) return;
            compile(sources::texture());
        }
    } inline texture_shader_object{ };
}