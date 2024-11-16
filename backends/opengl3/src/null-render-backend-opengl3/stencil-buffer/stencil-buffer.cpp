#include "../wrapper/opengl3.h"
#include "stencil-buffer.h"

namespace ntl::render::opengl3 {
    void c_stencil_buffer::create() {
        if(!empty()) return;

        opengl::gen_renderbuffers(1, &buffer);
        opengl::bind_renderbuffer(opengl::e_renderbuffer, buffer);
        opengl::renderbuffer_storage_multisample(opengl::e_renderbuffer, shared::msaa_quality, opengl::e_depth24_stencil8, shared::viewport.x, shared::viewport.y);
    }

    void c_stencil_buffer::destroy() {
        opengl::delete_renderbuffers(1, &buffer);
        buffer = 0;
    }

    void c_stencil_buffer::clear() {
        opengl::clear(opengl::e_stencil_buffer_bit);
    }
}