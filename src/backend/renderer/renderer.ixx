module;
#include <format>
#include <memory>
export module null.render:backend.renderer;

import null.sdk;

import :font.atlas;
//import :graphic.draw_list;
import :backend.renderer.object;
import :backend.renderer.mesh;
import :backend.renderer.frame_buffer;
import :backend.shaders.passthrough;

export namespace null::render::backend {
	class i_renderer {
	private:
		matrix4x4_t matrix{ };

	public:
		std::uint8_t msaa_quality{ 8 };
		vec2_t<float> viewport{ };

	public:
		void set_matrix(const matrix4x4_t& _matrix) { matrix = _matrix; }
		const matrix4x4_t& get_matrix() const { return matrix; }

	public:
		virtual matrix4x4_t get_projection_matrix() const = 0;

		virtual void set_texture(void* texture) = 0;
		virtual void set_clip(const rect_t<float>& rect) = 0;
		virtual void draw_geometry(size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) = 0;

		virtual void* create_texture(const vec2_t<float>& size, void* data) = 0;
		virtual void destroy_texture(void* texture) = 0;

		virtual void setup_state() = 0;

	private:
		virtual void save_state() { }
		virtual void restore_state() { }

		virtual void create_internal_objects() { }
		virtual void destroy_internal_objects() { }

	public:
		void begin_render() {
			save_state();

			object_event_dispatcher.begin_render();

			mesh->compile();

			setup_state();

			shaders::passthrough_color->use();

			if(msaa_quality != 0) {
				msaa_buffer->set();
				msaa_buffer->clear();
				msaa_buffer->copy_from(rendering_buffer);
			}

			//background.handle();
			//background.clear();
		}

		void end_render() {
			//foreground.handle();
			//foreground.clear();

			if(msaa_quality != 0) {
				rendering_buffer->set();
				rendering_buffer->copy_from(msaa_buffer);
			}

			mesh->clear_geometry();

			object_event_dispatcher.end_render();

			restore_state();
		}

		void create_objects() {
			if(empty()) return;

			object_event_dispatcher.create();

			create_atlases();
			create_internal_objects();
		}

		void destroy_objects() {
			if(empty()) return;

			object_event_dispatcher.destroy();

			destroy_atlases();
			destroy_internal_objects();
		}

	public:
		void create_atlases() {
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

		void destroy_atlases() {
			for(c_atlas* atlas : atlases_handler.atlases)
				destroy_texture(atlas->texture.data);
			atlases_handler.changed = true;
		}

	public:
		virtual bool empty() const = 0;
	}; std::unique_ptr<i_renderer> renderer{ };
}