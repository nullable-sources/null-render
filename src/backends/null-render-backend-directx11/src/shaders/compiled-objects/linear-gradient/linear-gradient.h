#pragma once
#include <wrapper/shaders/pixel-shader/pixel-shader.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::directx11::shaders::compiled_objects {
    namespace sources {
        static const std::vector<byte>& linear_gradient() {
#include <shaders/compiled-objects/linear-gradient/compiled/linear-gradient.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_linear_gradient : public wrapper::c_pixel_shader {
    public:
        struct constant_buffer_t {
            std::array<color_t<int>, 16> colors{ };
            std::array<vec4_t<float>, 16> stops{ };
            float angle{ };
            int stops_count{ };
        };
        wrapper::c_constant_buffer<constant_buffer_t> constant_buffer{ };

    public:
        void set_constant(const constant_buffer_t& constant, int slot = 0) {
            constant_buffer.edit_constant(constant);
            set_constant_buffer(constant_buffer.buffer, slot);
        }

    public:
        void on_create() override {
            if(!empty()) return;
            create(sources::linear_gradient());
            constant_buffer.create();
        }

        void on_destroy() override { destroy(); constant_buffer.destroy(); }
    } inline linear_gradient{ };
}