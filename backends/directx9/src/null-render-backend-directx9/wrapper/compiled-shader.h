#pragma once
#include <null-render.h>

namespace ntl::render::directx9 {
    class i_compiled_shader : public backend::i_compiled_shader_object {
    public:
        virtual void create() override {
            if(!empty()) return;
            compile(get_source());
        }

        virtual const byte* get_source() = 0;
        virtual void compile(const byte* source) = 0;

        virtual void set_constant(int location, const float* params, int count = 1) = 0;
        virtual void set_constant(int location, const int* params, int count = 1) = 0;

    public:
        virtual void set() = 0;
    };
}