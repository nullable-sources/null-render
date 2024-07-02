#pragma once
#include "../color/compiled-object.h"

namespace null::render::directx9 {
    namespace sources {
        static const std::vector<byte>& sdf() {
#include "compiled/sdf.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }

        static const std::vector<byte>& msdf() {
#include "compiled/msdf.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }

        static const std::vector<byte>& mtsdf() {
#include "compiled/mtsdf.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_sdf_shader_object : public c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::sdf().data(); }
    } inline sdf_shader_object{ };

    class c_msdf_shader_object : public c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::msdf().data(); }
    } inline msdf_shader_object{ };

    class c_mtsdf_shader_object : public c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::mtsdf().data(); }
    } inline mtsdf_shader_object{ };
}