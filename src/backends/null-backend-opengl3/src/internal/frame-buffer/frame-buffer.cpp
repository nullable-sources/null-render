#include <null-render.h>
#include <wrapper/opengl3.h>

#include <internal/frame-buffer/frame-buffer.h>

namespace null::render::backend::opengl3 {
	void c_frame_buffer::create() {
		if(!empty()) return;

		if(type == e_frame_buffer_type::postprocessing) {
			opengl::gen_framebuffers(1, &fbo);
			opengl::bind_framebuffer(opengl::e_framebuffer, fbo);

			//@note: create a color attachment texture
			opengl::gen_textures(1, &fbo_texture);
			if(flags & e_frame_buffer_flags::msaa) {
				opengl::bind_texture(opengl::e_texture_2d_multisample, fbo_texture);
				
				opengl::tex_image2d_multisample(opengl::e_texture_2d_multisample, shared::msaa_quality, opengl::e_rgb, shared::viewport.x, shared::viewport.y, true);
				
				opengl::bind_texture(opengl::e_texture_2d_multisample, 0);
				opengl::framebuffer_texture2d(opengl::e_framebuffer, opengl::e_color_attachment0, opengl::e_texture_2d_multisample, fbo_texture, 0);
			} else {
				opengl::bind_texture(opengl::e_texture_2d, fbo_texture);
				
				opengl::tex_image2d(opengl::e_texture_2d, 0, opengl::e_rgb, shared::viewport.x, shared::viewport.y, 0, opengl::e_rgb, opengl::e_unsigned_byte, NULL);
				opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_min_filter, opengl::e_linear);
				opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_mag_filter, opengl::e_linear);
				
				opengl::bind_texture(opengl::e_texture_2d, 0);
				opengl::framebuffer_texture2d(opengl::e_framebuffer, opengl::e_color_attachment0, opengl::e_texture_2d, fbo_texture, 0);
			}

			//@note: create a renderbuffer object for depth and stencil attachments
			opengl::gen_renderbuffers(1, &rbo);
			opengl::bind_renderbuffer(opengl::e_renderbuffer, rbo);
			if(flags & e_frame_buffer_flags::msaa) {
				opengl::renderbuffer_storage_multisample(opengl::e_renderbuffer, shared::msaa_quality, opengl::e_depth24_stencil8, shared::viewport.x, shared::viewport.y);
			} else {
				opengl::renderbuffer_storage(opengl::e_renderbuffer, opengl::e_depth24_stencil8, shared::viewport.x, shared::viewport.y);
			}
			opengl::bind_renderbuffer(opengl::e_renderbuffer, 0);
			opengl::framebuffer_renderbuffer(opengl::e_framebuffer, opengl::e_depth_stencil_attachment, opengl::e_renderbuffer, rbo);

			opengl::bind_framebuffer(opengl::e_framebuffer, 0);

		} else {
			opengl::get_integerv(opengl::e_draw_framebuffer_binding, (int*)&fbo);
			opengl::get_integerv(opengl::e_renderbuffer_binding, (int*)&rbo);
		}
	}

	void c_frame_buffer::destroy() {
		opengl::delete_framebuffers(1, &fbo);
		opengl::delete_textures(1, &fbo_texture);
		opengl::delete_renderbuffers(1, &rbo);
	}

	void c_frame_buffer::clear() {
		opengl::clear_color(0, 0, 0, 0);
		opengl::clear(opengl::e_color_buffer_bit | opengl::e_depth_buffer_bit);
	}

	void c_frame_buffer::set() {
		opengl::bind_framebuffer(opengl::e_framebuffer, fbo);
		opengl::bind_renderbuffer(opengl::e_renderbuffer, rbo);
	}

	void c_frame_buffer::copy_from_backbuffer() {
		opengl::bind_framebuffer(opengl::e_draw_framebuffer, fbo);
		opengl::read_buffer(opengl::e_back);
		opengl::draw_buffer(opengl::e_color_attachment0);
		opengl::blit_framebuffer(0, 0, shared::viewport.x, shared::viewport.y, 0, 0, shared::viewport.x, shared::viewport.y, opengl::e_color_buffer_bit, opengl::e_linear);
	}

	void c_frame_buffer::copy_from_another_frame_buffer(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) {
		opengl::bind_framebuffer(opengl::e_read_framebuffer, (std::uint32_t)another_frame_buffer->get_texture());
		opengl::bind_framebuffer(opengl::e_draw_framebuffer, fbo);
		opengl::blit_framebuffer(0, 0, shared::viewport.x, shared::viewport.y, 0, 0, shared::viewport.x, shared::viewport.y, opengl::e_color_buffer_bit, opengl::e_linear);
	}
}