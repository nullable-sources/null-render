#include "../../../backend/renderer/renderer.h"
#include "../../../backend/state-pipeline/state-pipeline.h"
#include "renderer.h"

namespace ntl::render {
    void c_texture_command::handle() {
        backend::renderer->set_texture(texture);
    }

    void c_clip_command::handle() {
        backend::renderer->set_clip(rect);
    }

    void c_set_matrix_command::handle() {
        backend::renderer->set_matrix(matrix);
    }

    void c_update_matrix_command::handle() {
        backend::renderer->update_matrix(matrix);
    }

    void c_set_translation_command::handle() {
        backend::renderer->set_translation(translation);
    }

    void c_update_translation_command::handle() {
        backend::renderer->update_translation(translation);
    }

    void c_set_depth_command::handle() {
        backend::renderer->set_depth(depth);
    }

    void c_update_depth_command::handle() {
        backend::renderer->update_depth(depth);
    }

    void c_stencil_test_command::handle() {
        backend::stencil_buffer->set_test(test);
    }

    void c_stencil_operation_command::handle() {
        backend::stencil_buffer->set_operation(operation);
    }

    void c_stencil_clear_command::handle() {
        backend::stencil_buffer->clear();
    }
}