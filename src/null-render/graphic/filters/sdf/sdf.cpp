#include "../../../font/font.h"
#include "../../../font/loaders/truetype/truetype.h"

#include "../../../backend/state-pipeline/state-pipeline.h"

#include "sdf.h"

namespace null::render {
    void c_sdf_filter::handle() {
        float aa = 0.25f / sdf_padding * float(sdf_pixel_size) / constants.aa;
        if(constants.aa < 16.f)
            aa *= std::pow(constants.aa / 16.f, 2);

        constants.aa = aa;
        constants.outline_thickness = std::clamp(0.5f - constants.outline_thickness / 2.0f, constants.aa, 0.5f);

        backend::sdf_shader->set_constants(constants);
        backend::state_pipeline->shaders.push(backend::sdf_shader);
        backend::state_pipeline->textures.push(texture);
        child_command->handle();
        backend::state_pipeline->textures.pop();
        backend::state_pipeline->shaders.pop();
    }
}