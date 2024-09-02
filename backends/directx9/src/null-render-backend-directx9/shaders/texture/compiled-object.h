#pragma once
#include "../color/compiled-object.h"

namespace ntl::render::directx9 {
    namespace sources {
        static const std::vector<byte>& texture() {
#include "compiled/texture.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_texture_shader_object : public c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::texture().data(); }
    } inline texture_shader_object{ };
}