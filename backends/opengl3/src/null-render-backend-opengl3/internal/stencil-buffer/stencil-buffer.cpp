#include "../../wrapper/opengl3.h"
#include "stencil-buffer.h"

namespace null::render::opengl3 {
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

	void c_stencil_buffer::set_test(bool test) {
		if(test) opengl::enable(opengl::e_stencil_test);
		else opengl::disable(opengl::e_stencil_test);
	}

	void c_stencil_buffer::set_operation(backend::e_stencil_operation operation) {
		if(operation == backend::e_stencil_operation::end_write) {
			opengl::color_mask(true, true, true, true);
			opengl::stencil_op(opengl::e_keep, opengl::e_keep, opengl::e_keep);
			opengl::stencil_func(opengl::e_equal, stencil_ref, std::uint32_t(-1));
			return;
		}

		opengl::color_mask(false, false, false, false);
		opengl::stencil_func(opengl::e_always, 1, std::uint32_t(-1));

		switch(operation) {
			case backend::e_stencil_operation::set:
			case backend::e_stencil_operation::set_inverse: {
				opengl::stencil_op(opengl::e_keep, opengl::e_keep, opengl::e_replace);
				stencil_ref = operation == backend::e_stencil_operation::set ? 1 : 0;
			} break;

			case backend::e_stencil_operation::intersect: {
				opengl::stencil_op(opengl::e_keep, opengl::e_keep, opengl::e_incr);
				stencil_ref++;
			} break;
		}
	}
}