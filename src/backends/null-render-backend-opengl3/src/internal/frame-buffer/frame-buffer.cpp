#include <null-render.h>
#include <wrapper/opengl3.h>

#include <internal/frame-buffer/frame-buffer.h>

namespace null::render::backend::opengl3 {
	void c_frame_buffer::create() {
		if(!empty()) return;

		if(type == e_frame_buffer_type::postprocessing) {
			opengl::gen_framebuffers(1, &fbo);
			opengl::bind_framebuffer(opengl::e_framebuffer, fbo);

			if(flags & e_frame_buffer_flags::msaa) {
				//@note: depth buffer
				if(flags & e_frame_buffer_flags::depth_buffer) {
					opengl::gen_renderbuffers(1, &depth_buffer);
					opengl::bind_renderbuffer(opengl::e_renderbuffer, depth_buffer);
					opengl::renderbuffer_storage_multisample(opengl::e_renderbuffer, shared::msaa_quality, opengl::e_depth_component, size.x, size.y);
					opengl::framebuffer_renderbuffer(opengl::e_framebuffer, opengl::e_depth_attachment, opengl::e_renderbuffer, depth_buffer);
				}

				//@note: renderbuffer
				opengl::gen_renderbuffers(1, &fbo_attachment);
				opengl::bind_renderbuffer(opengl::e_renderbuffer, fbo_attachment);
				opengl::renderbuffer_storage_multisample(opengl::e_renderbuffer, shared::msaa_quality, opengl::e_rgba, size.x, size.y);
				opengl::framebuffer_renderbuffer(opengl::e_framebuffer, opengl::e_color_attachment0, opengl::e_renderbuffer, fbo_attachment);
			} else {
				//@note: depth buffer
				if(flags & e_frame_buffer_flags::depth_buffer) {
					opengl::gen_textures(1, &depth_buffer);
					opengl::bind_texture(opengl::e_texture_2d, depth_buffer);

					opengl::tex_parameterf(opengl::e_texture_2d, opengl::e_texture_wrap_s, opengl::e_clamp_to_edge);
					opengl::tex_parameterf(opengl::e_texture_2d, opengl::e_texture_wrap_t, opengl::e_clamp_to_edge);
					opengl::tex_parameterf(opengl::e_texture_2d, opengl::e_texture_mag_filter, opengl::e_linear);
					opengl::tex_parameterf(opengl::e_texture_2d, opengl::e_texture_min_filter, opengl::e_linear);
					opengl::tex_image2d(opengl::e_texture_2d, 0, opengl::e_depth_component, size.x, size.y, 0, opengl::e_depth_component, opengl::e_unsigned_byte, NULL);
					
					opengl::framebuffer_texture2d(opengl::e_framebuffer, opengl::e_depth_attachment, opengl::e_texture_2d, depth_buffer, 0);
				}

				//@note: texture
				opengl::gen_textures(1, &fbo_attachment);
				opengl::bind_texture(opengl::e_texture_2d, fbo_attachment);
				
				opengl::tex_image2d(opengl::e_texture_2d, 0, opengl::e_rgba, size.x, size.y, 0, opengl::e_rgba, opengl::e_unsigned_byte, nullptr);
				opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_min_filter, opengl::e_linear);
				opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_mag_filter, opengl::e_linear);
				opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_wrap_s, opengl::e_clamp_to_edge);
				opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_wrap_t, opengl::e_clamp_to_edge);

				opengl::bind_texture(opengl::e_texture_2d, 0);
				opengl::framebuffer_texture2d(opengl::e_framebuffer, opengl::e_color_attachment0, opengl::e_texture_2d, fbo_attachment, 0);
			}

			opengl::bind_framebuffer(opengl::e_framebuffer, 0);
		} else {
			opengl::get_integerv(opengl::e_draw_framebuffer_binding, (int*)&fbo);
		}
	}

	void c_frame_buffer::destroy() {
		if(flags & e_frame_buffer_flags::depth_buffer) opengl::delete_renderbuffers(1, &depth_buffer);

		if(flags & e_frame_buffer_flags::msaa) opengl::delete_renderbuffers(1, &fbo_attachment);
		else opengl::delete_textures(1, &fbo_attachment);
		opengl::delete_framebuffers(1, &fbo);
	}

	void c_frame_buffer::clear() {
		opengl::clear_color(0.f, 0.f, 0.f, 0.f);
		opengl::clear(opengl::e_color_buffer_bit | opengl::e_depth_buffer_bit);
	}

	void c_frame_buffer::set() {
		opengl::bind_framebuffer(opengl::e_framebuffer, fbo);
	}

	void c_frame_buffer::copy_from(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) {
		opengl::bind_framebuffer(opengl::e_read_framebuffer, (std::uint32_t)another_frame_buffer->get_surface());
		opengl::bind_framebuffer(opengl::e_draw_framebuffer, fbo);
		opengl::blit_framebuffer(0, 0, another_frame_buffer->size.x, another_frame_buffer->size.y, 0, 0, size.x, size.y, opengl::e_color_buffer_bit | opengl::e_depth_buffer_bit, opengl::e_nearest);
	}
}