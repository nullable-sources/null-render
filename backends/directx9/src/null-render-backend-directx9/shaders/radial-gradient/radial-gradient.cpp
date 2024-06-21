#include "radial-gradient.h"

namespace null::render::directx9 {
    void c_radial_gradient_shader::set_constants(const constants_t& constants) {
        std::array<color_t<float>, 16> colors{ };
        std::array<vec4_t<float>, 16> stops{ };

        for(auto [color, stop, pair] : std::views::zip(colors, stops, constants.stops)) {
            color = pair.first.cast<float>();
            stop.x = pair.second;
        }

        const float stops_count = (float)constants.stops.size();
        pixel_shader->set_constant(0, &stops_count);
        pixel_shader->set_constant(1, (float*)&constants.origin);
        pixel_shader->set_constant(2, (float*)&constants.radius);
        pixel_shader->set_constant(3, (float*)stops.data(), 4);
        pixel_shader->set_constant(19, (float*)colors.data(), 16);
    }
}