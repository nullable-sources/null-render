#pragma once
#include "../internal/shader.h"

namespace null::render::backend {
    class i_quad_gradient_shader : public virtual i_shader {
    public:
        struct constants_t {
        public:
            std::array<color_t<int>, 4> colors{ };
        };

    public:
        virtual void set_constants(const constants_t& constants) = 0;
    }; inline std::unique_ptr<i_quad_gradient_shader> quad_gradient_shader{ };
}