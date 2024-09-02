#pragma once
#include "../color/compiled-object.h"

namespace ntl::render::opengl3 {
    class c_sdf_shader_object : public c_fragment_shader {
    private:
        std::string src_resource_name{ };

    public:
        c_sdf_shader_object(std::string_view _src_resource_name) : src_resource_name(_src_resource_name) { }

    public:
        ntl::mem::resource_t get_source() override {
            return ntl::mem::resource_t(src_resource_name, "null-render:opengl:resources");
        }
    } inline sdf_shader_object{ "null-render:opengl:shaders:sdf.frag" },
        msdf_shader_object{ "null-render:opengl:shaders:msdf.frag" },
        mtsdf_shader_object{ "null-render:opengl:shaders:mtsdf.frag" };
}