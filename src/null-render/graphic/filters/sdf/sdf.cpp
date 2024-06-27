#include "../../../font/font.h"
#include "../../../font/loaders/truetype/truetype.h"

#include "../../../backend/state-pipeline/state-pipeline.h"

#include "sdf.h"

namespace null::render {
    void c_sdf_filter::set_default_aa(float dst_aa) {
        float aa = 0.25f / sdf_padding * float(sdf_pixel_size) / dst_aa;
        if(dst_aa < 16.f)
            aa *= std::pow(dst_aa / 16.f, 2);

        constants.aa = aa;
        constants.outline_thickness = std::clamp(0.5f - constants.outline_thickness / 2.0f, constants.aa, 0.5f);
    }

    void c_sdf_filter::handle() {
        backend::sdf_shader->set_constants(constants);
        backend::state_pipeline->shaders.push(backend::sdf_shader);
        backend::state_pipeline->textures.push(texture);
        child_command->handle();
        backend::state_pipeline->textures.pop();
        backend::state_pipeline->shaders.pop();
    }
}