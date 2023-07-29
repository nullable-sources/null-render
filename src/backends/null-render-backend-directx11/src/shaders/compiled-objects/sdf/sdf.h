#pragma once
#include <wrapper/shaders/pixel-shader/pixel-shader.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::directx11::shaders::compiled_objects {
    namespace sources {
        static const std::vector<byte>& sdf() {
            #include <shaders/compiled-objects/sdf/compiled/sdf.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_sdf : public wrapper::c_pixel_shader {
    public:
        struct constant_buffer_t {
            color_t<int> outline_start{ }, outline_end{ };
            float aa{ }, outline_thickness{ };
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
            create(sources::sdf());
            constant_buffer.create();
        }

        void on_destroy() override { destroy(); constant_buffer.destroy(); }
    } inline sdf{ };
}