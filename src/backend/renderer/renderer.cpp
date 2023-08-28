#include "null-render.h"

#include "backend/renderer/renderer.h"
#include "backend/internal/mesh.h"
#include "backend/internal/frame-buffer.h"
#include "backend/shaders/passthrough-shader.h"

namespace null::render::backend {
	void i_renderer::create_objects() {
		if(empty()) return;

		object_event_dispatcher.create();

		create_atlases();
		create_internal_objects();
	}

	void i_renderer::destroy_objects() {
		if(empty()) return;

		object_event_dispatcher.destroy();

		destroy_atlases();
		destroy_internal_objects();
	}

	void i_renderer::begin_render() {
		save_state();

		object_event_dispatcher.begin_render();

		mesh->compile();

		setup_state();

		passthrough_color_shader->use();

		if(render::shared::msaa_quality != 0) {
			msaa_buffer->set();
			msaa_buffer->clear();
			msaa_buffer->copy_from(rendering_buffer);
		}

		background.handle();
		background.clear();
	}

	void i_renderer::end_render() {
		foreground.handle();
		foreground.clear();

		if(render::shared::msaa_quality != 0) {
			rendering_buffer->set();
			rendering_buffer->copy_from(msaa_buffer);
		}

		mesh->clear_geometry();

		object_event_dispatcher.end_render();

		restore_state();
	}

	void i_renderer::create_atlases() {
		if(!atlases_handler.changed) return;

		for(c_atlas* atlas : atlases_handler.atlases) {
			if(atlas->texture.data) destroy_texture(atlas->texture.data);

			if(atlas->texture.pixels_alpha8.empty()) {
				if(atlas->configs.empty()) atlas->add_font_default();
				atlas->build();
			}

			atlas->texture.get_data_as_rgba32();
			atlas->texture.data = create_texture(atlas->texture.size, atlas->texture.pixels_rgba32.data());
		}

		atlases_handler.changed = false;
	}

	void i_renderer::destroy_atlases() {
		for(c_atlas* atlas : atlases_handler.atlases) destroy_texture(atlas->texture.data);
		atlases_handler.changed = true;
	}
}