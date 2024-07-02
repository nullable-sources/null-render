#pragma once
#include "../color/compiled-object.h"

namespace null::render::directx11 {
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

    template <typename sdf_shader_t>
    class i_sdf_shader_object : public c_pixel_shader {
    private:
        const std::vector<byte>* sdf_data{ };

    public:
        i_sdf_shader_object(const std::vector<byte>& _sdf_data) : sdf_data(&_sdf_data) { }

    public:
        c_constant_buffer<typename sdf_shader_t::constants_t> constant_buffer{ };

    public:
        void set_constant(const sdf_shader_t::constants_t& constant, int slot = 0) {
            constant_buffer.edit_constant(constant);
            set_constant_buffer(constant_buffer.buffer, slot);
        }

    public:
        void create() override {
            if(!empty()) return;
            compile(*sdf_data);
            constant_buffer.create();
        }

        void destroy() override { c_pixel_shader::destroy(); constant_buffer.destroy(); }
    };

    inline i_sdf_shader_object<backend::i_sdf_shader> sdf_shader_object{ sources::sdf() };
    inline i_sdf_shader_object<backend::i_msdf_shader> msdf_shader_object{ sources::msdf() };
    inline i_sdf_shader_object<backend::i_mtsdf_shader> mtsdf_shader_object{ sources::mtsdf() };
}