#include "../internal/frame-buffer.h"
#include "../state-pipeline/state-pipeline.h"
#include "../post-processing/post-processing.h"

#include "../../graphic/draw-list/draw-list.h"

#include "renderer.h"

namespace null::render::backend {
	void i_renderer::update_uniforms() {
		if(state_pipeline->shaders.empty()) return;
		state_pipeline->shaders.append_last();
	}

	void i_renderer::create_objects() {
		if(empty()) return;

		create_atlases();
		create_internal_objects();
	}

	void i_renderer::destroy_objects() {
		if(empty()) return;

		renderer_event_dispatcher.destroy();

		destroy_atlases();
		destroy_internal_objects();
	}

	void i_renderer::begin_resize_viewport(const vec2_t<int>& new_viewport) {
		shared::viewport = new_viewport;
		renderer_event_dispatcher.viewport_resize_begin();
	}

	void i_renderer::end_resize_viewport() {
		renderer_event_dispatcher.viewport_resize_end();
	}

	std::unique_ptr<std::uint8_t[]> i_renderer::premultiply_texture_color(const vec2_t<float>& size, std::uint8_t* data) {
		const size_t data_size = size.x * size.y * 4;
		std::unique_ptr<std::uint8_t[]> premultiplied(new std::uint8_t[data_size]);
		if(!data) return std::move(premultiplied);

		for(size_t i : std::views::iota(0u, data_size) | std::views::stride(4u)) {
			const std::uint8_t alpha = data[i + 3];
			for(size_t j : std::views::iota(0u, 3u))
				premultiplied[i + j] = int(data[i + j]) * int(alpha) / 255;
			premultiplied[i + 3] = alpha;
		}

		return std::move(premultiplied);
	}

	void i_renderer::begin_render() {
		renderer_event_dispatcher.begin_render();

		state_pipeline->save_state();

        renderer_event_dispatcher.create();

		state_pipeline->setup_state();

		if(render::shared::msaa_quality != 0) {
			state_pipeline->framebuffers.push(msaa_buffer);
			msaa_buffer->clear();
		}

		draw_list->compile();
		draw_list->handle();
		draw_list->clear();
	}

	void i_renderer::end_render() {
		if(render::shared::msaa_quality != 0) {
			state_pipeline->framebuffers.pop();

			post_processing->blit_buffer(msaa_buffer.get());
		}

		renderer_event_dispatcher.end_render();

		state_pipeline->restore_state();
	}

	void i_renderer::create_atlases() {
		if(!atlases_handler.changed) return;

		for(c_atlas* atlas : atlases_handler.atlases) {
			c_atlas::texture_t& texture = atlas->texture;
			if(texture.data) destroy_texture(texture.data);

			if(texture.pixels_alpha8.empty()) {
				if(atlas->configs.empty()) atlas->add_font_default();
				atlas->build();
			}

			texture.get_data_as_rgba32();
			texture.data = create_texture(texture.size, texture.pixels_rgba32.data());
		}

		atlases_handler.changed = false;
	}

	void i_renderer::destroy_atlases() {
		for(c_atlas* atlas : atlases_handler.atlases) {
			destroy_texture(atlas->texture.data);
			atlas->texture.data = nullptr;
		}
		atlases_handler.changed = true;
	}
}