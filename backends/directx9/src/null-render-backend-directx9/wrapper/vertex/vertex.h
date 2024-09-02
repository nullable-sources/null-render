#pragma once
#include "../../renderer/renderer.h"
#include "../compiled-shader.h"

namespace ntl::render::directx9 {
    class c_vertex_shader : public i_compiled_shader {
    public:
        IDirect3DVertexShader9* shader{ };

    public:
        virtual void destroy() override;

        virtual void compile(const byte* source) override;

        virtual void set_constant(int location, const float* params, int count = 1) override;
        virtual void set_constant(int location, const int* params, int count = 1) override;

    public:
        virtual void set() override;

        virtual bool empty() const override { return !shader; }
    };
}