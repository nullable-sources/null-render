#include "shader.h"

namespace ntl::render::opengl3 {
    void c_shader::create() {
        if(!empty()) return;
        program = std::make_unique<c_program>();
        program->create();

        program->attach_shader(fragment_shader);
        program->attach_shader(vertex_shader);

        program->link();

        program->detach_shader(vertex_shader);
        program->detach_shader(fragment_shader);
    }

    void c_shader::destroy() {
        program->destroy();
    }

    void c_shader::use() {
        program->use();
    }

    void c_default_shader::create() {
        if(!empty()) return;
        c_shader::create();

        matrix.get_location(program.get(), "matrix");
        translation.get_location(program.get(), "translation");
        depth.get_location(program.get(), "depth");
    }

    void c_default_shader::use() {
        c_shader::use();

        matrix.set(backend::renderer->get_matrix());
        translation.set(backend::renderer->get_translation());
        depth.set(backend::renderer->get_depth());
    }
}