#pragma once
#include <wrapper/shaders/pixel-shader/pixel-shader.h>
#include <backend/shaders/shader.h>

namespace null::render::directx11 {
    namespace sources {
        static const std::vector<byte>& quad_gradient() {
            #include <shaders/compiled-objects/quad-gradient/compiled/quad-gradient.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
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
        void on_create() override {
            if(!empty()) return;
            create(sources::quad_gradient());
            constant_buffer.create();
        }

        void on_destroy() override { destroy(); constant_buffer.destroy(); }
    } inline quad_gradient_shader_object{ };
}