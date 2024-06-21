#include "sdf.h"

namespace null::render::directx9 {
    void c_sdf_shader::set_constants(const constants_t& constants) {
        pixel_shader->set_constant(0, &constants.aa);
        pixel_shader->set_constant(1, &constants.outline_thickness);
        pixel_shader->set_constant(2, constants.outline_start.cast<float>().channels.data());
        pixel_shader->set_constant(3, constants.outline_end.cast<float>().channels.data());
    }
}