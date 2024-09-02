#pragma once
#include <null-render.h>
#include "opengl3.h"

namespace ntl::render::opengl3 {
    class i_compiled_shader : public backend::i_compiled_shader_object {
    public:
        std::uint32_t shader{ };

    public:
        virtual void destroy() override {
            if(shader) {
                opengl::delete_shader(shader); check(opengl::e_delete_status, "destroy"); shader = 0;
            }
        }

        virtual resource_t get_source() = 0;
        virtual void compile(const resource_t& resource) { compile((const char*)resource.locked_data, resource.locked_data_size); }
        virtual void compile(const char* source, int length) {
            opengl::shader_source(shader, 1, (const char* const*)&source, &length);
            opengl::compile_shader(shader);
            check(opengl::e_compile_status, "compile");
        }

        virtual bool empty() const override { return shader == 0; }

    public:
        void check(opengl::e_constants status, std::string_view desc) {
            std::string log = "empty";

            int result_status{ }, log_length{ };
            opengl::get_shaderiv(shader, status, &result_status);
            opengl::get_shaderiv(shader, opengl::e_info_log_length, &log_length);

            if(log_length > 1) {
                log = std::string(log_length + 1, char{ });
                opengl::get_shader_info_log(shader, log_length, nullptr, log.data());
            }

            if(!result_status) sdk::logger(sdk::e_log_type::warning, "cant '{}' shader, log \"{}\".", desc, log);
        }

    public:
        virtual operator bool() const { return !empty(); }
    };

    class c_vertex_shader : public i_compiled_shader {
    public:
        virtual void create() override {
            if(!empty()) return;
            shader = opengl::create_shader(opengl::e_vertex_shader);
            compile(get_source().load());
        }
    };

    class c_fragment_shader : public i_compiled_shader {
    public:
        virtual void create() override {
            if(!empty()) return;
            shader = opengl::create_shader(opengl::e_fragment_shader);
            compile(get_source().load());
        }
    };
}