#pragma once
#include "../internal/shader.h"

namespace null::render::backend {
    //@note: https://github.com/Chlumsky/msdfgen
    //       https://github.com/Chlumsky/msdf-atlas-gen
    //       https://www.redblobgames.com/x/2404-distance-field-effects/
    //
    //       For more understanding, follow the links above

    class i_sdf_shader : public virtual i_shader {
    public:
        struct constants_t {
        public:
            color_t<int> outline_color{ };
            float outline_blur{ }, outline_width_relative{ }, outline_width_absolute{ };
            float threshold{ }, out_bias{ };
            float px_range{ };
        };

    public:
        virtual void set_constants(const constants_t& constants) = 0;
    }; inline std::unique_ptr<i_sdf_shader> sdf_shader{ };

    class i_msdf_shader : public i_sdf_shader {
    public:
        virtual void set_constants(const constants_t& constants) = 0;
    }; inline std::unique_ptr<i_msdf_shader> msdf_shader{ };

    class i_mtsdf_shader : public virtual i_shader {
    public:
        struct constants_t : public i_sdf_shader::constants_t {
        public:
            float glyph_rounding{ }, outline_rounding{ };
        };

    public:
        virtual void set_constants(const constants_t& constants) = 0;
    }; inline std::unique_ptr<i_mtsdf_shader> mtsdf_shader{ };
}