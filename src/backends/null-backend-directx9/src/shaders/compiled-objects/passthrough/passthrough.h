#pragma once
#include <wrapper/vertex/vertex.h>
#include <backend/shaders/shader.h>

namespace null::render::backend::directx9::shaders::compiled_objects {
    namespace sources {
        static const std::vector<byte>& passthrough() {
            #include <shaders/compiled-objects/passthrough/compiled/passthrough.h>
            static const std::vector<byte> source{ shader_data, shader_data + sizeof(shader_data) };
            return source;
        }
    }

    class c_passthrough : public wrapper::c_vertex_shader {
    public:
        void on_create() override {
            if(!empty()) return;
            compile(sources::passthrough().data());
        }
    } inline passthrough{ };
}