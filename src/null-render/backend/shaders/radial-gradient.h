#pragma once
#include "../internal/shader.h"

namespace null::render::backend {
    class i_radial_gradient_shader : public virtual i_shader {
    public:
        struct constants_t {
        public:
            vec2_t<float> origin{ };
            vec2_t<float> radius{ };
            std::vector<std::pair<color_t<int>, float>> stops{ };
        };

    public:
        virtual void set_constants(const constants_t& constants) = 0;
    }; inline std::unique_ptr<i_radial_gradient_shader> radial_gradient_shader{ };
}