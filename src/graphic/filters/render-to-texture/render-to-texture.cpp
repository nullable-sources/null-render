#include <backend/renderer/renderer.h>
#include <backend/internal/frame-buffer.h>

#include <graphic/filters/render-to-texture/render-to-texture.h>

namespace null::render::filters {
	void c_render_to_texture::handle() {
		backend::rtt_buffer->set();
		backend::rtt_buffer->clear();

		child_command->handle();

		backend::msaa_buffer->set();
	}
}