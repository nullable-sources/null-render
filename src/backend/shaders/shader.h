#pragma once
#include <backend/internal/object.h>

namespace null::render::backend::shaders {
    class i_shader : public i_object {
    public:
        virtual void use() = 0;
        virtual bool empty() const = 0;
    };

    //@note: compiled shader object
    class i_compiled_object : i_object {
    public:
        virtual bool empty() const = 0;
    };
}