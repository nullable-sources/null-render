#pragma once
#include "../color/compiled-object.h"

namespace ntl::render::directx11 {
    namespace sources {
        static const std::vector<byte>& quad_gradient() {
#include "compiled/quad-gradient.h"
            static const std::vector<byte> source(shader_data, shader_data + sizeof(shader_data));
            return source;
        }
    }

    class c_quad_gradient_shader_object : public c_pixel_shader {
    public:
        c_constant_buffer<backend::i_quad_gradient_shader::constants_t> constant_buffer{ };

    public:
        void set_constant(const backend::i_quad_gradient_shader::constants_t& constant, int slot = 0) {
            constant_buffer.edit_constant(constant);
            set_constant_buffer(constant_buffer.buffer, slot);
        }

    public:
        void create() override {
            if(!empty()) return;
            compile(sources::quad_gradient());
            constant_buffer.create();
        }

        void destroy() override { c_pixel_shader::destroy(); constant_buffer.destroy(); }
    } inline quad_gradient_shader_object{ };
}