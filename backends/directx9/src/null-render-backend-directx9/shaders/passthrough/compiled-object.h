#pragma once
#include "../../wrapper/pixel/pixel.h"

namespace ntl::render::directx9 {
    namespace sources {
        static const std::vector<byte>& passthrough_ps() {
#include "compiled/passthrough-ps.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }

        static const std::vector<byte>& passthrough_vs() {
#include "compiled/passthrough-vs.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_passthrough_pixel_shader_object : public c_pixel_shader {
    public:
        virtual const byte* get_source() override { return sources::passthrough_ps().data(); }
    } inline passthrough_pixel_shader_object{ };

    class c_passthrough_vertex_shader_object : public c_vertex_shader {
    public:
        virtual const byte* get_source() override { return sources::passthrough_vs().data(); }
    } inline passthrough_vertex_shader_object{ };
}