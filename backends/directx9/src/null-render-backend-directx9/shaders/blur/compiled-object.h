#pragma once
#include "../passthrough/compiled-object.h"

namespace ntl::render::directx9 {
    namespace sources {
        static const std::vector<byte>& blur() {
#include "compiled/blur.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_blur_shader_object : public c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::blur().data(); }
    } inline blur_shader_object{ };
}