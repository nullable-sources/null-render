#include <backend/internal/frame-buffer.h>
#include <graphic/commands/postprocessing-commands/postprocessing-commands.h>

namespace null::render::commands {
	void c_rtt_to_postprocess::handle() {
		backend::postprocessing_buffers[0]->set();
		backend::postprocessing_buffers[0]->clear();

		backend::postprocessing_buffers[0]->copy_from(backend::rtt_buffer);

		backend::msaa_buffer->set();
	}

	void c_backbuffer_to_postprocess::handle() {
		backend::postprocessing_buffers[0]->set();
		//backend::postprocessing_buffers[0]->clear();

		backend::postprocessing_buffers[0]->copy_from(backend::msaa_buffer);

		backend::msaa_buffer->set();
	}
}