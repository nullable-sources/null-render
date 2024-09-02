#include "../../../font/font.h"
#include "../../../backend/state-pipeline/state-pipeline.h"
#include "sdf.h"

namespace ntl::render {
    void c_sdf_filter::handle() {
        backend::sdf_shader->set_constants(constants);
        backend::state_pipeline->shaders.push(backend::sdf_shader);
        backend::state_pipeline->textures.push(texture);
        child_command->handle();
        backend::state_pipeline->textures.pop();
        backend::state_pipeline->shaders.pop();
    }

    void c_msdf_filter::handle() {
        backend::msdf_shader->set_constants(constants);
        backend::state_pipeline->shaders.push(backend::msdf_shader);
        backend::state_pipeline->textures.push(texture);
        child_command->handle();
        backend::state_pipeline->textures.pop();
        backend::state_pipeline->shaders.pop();
    }

    void c_mtsdf_filter::handle() {
        backend::mtsdf_shader->set_constants(constants);
        backend::state_pipeline->shaders.push(backend::mtsdf_shader);
        backend::state_pipeline->textures.push(texture);
        child_command->handle();
        backend::state_pipeline->textures.pop();
        backend::state_pipeline->shaders.pop();
    }
}