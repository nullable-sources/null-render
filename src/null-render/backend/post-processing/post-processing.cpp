
#include "../backend.h"

namespace null::render::backend {
	void c_post_processing::initialize() {
		for(auto& buffer : post_processing->frame_buffers) {
			buffer = factory->instance_frame_buffer(shared::viewport, e_frame_buffer_type::postprocessing, backend::e_frame_buffer_flags::none);
		}

		transfer_buffer = factory->instance_frame_buffer(shared::viewport, e_frame_buffer_type::postprocessing, e_frame_buffer_flags::none);
	}

	void c_post_processing::blit_buffer(const std::unique_ptr<i_frame_buffer>& buffer) {
		void* texture = prepare_buffer_texture(buffer);

		if(geometry_dirty)
			generate_viewport_geometry();

		state_pipeline->textures.push(texture);
		state_pipeline->shaders.push(passthrough_texture_shader);
		draw_geometry();
		state_pipeline->shaders.pop();
		state_pipeline->textures.pop();
	}

	void c_post_processing::blit_buffer_region(const std::unique_ptr<i_frame_buffer>& buffer, const rect_t<float>& geometry_region, const rect_t<float>& uvs_region) {
		void* texture = prepare_buffer_texture(buffer);

		geometry_dirty = true;
		generate_geometry(geometry_region, uvs_region);

		state_pipeline->textures.push(texture);
		state_pipeline->shaders.push(passthrough_texture_shader);
		draw_geometry();
		state_pipeline->shaders.pop();
		state_pipeline->textures.pop();
	}

	void* c_post_processing::prepare_buffer_texture(const std::unique_ptr<i_frame_buffer>& buffer) {
		void* texture = buffer->get_texture();
		if(buffer->flags & e_frame_buffer_flags::msaa) {
			transfer_buffer->clear();
			transfer_buffer->copy_from(buffer);
			texture = transfer_buffer->get_texture();
		}

		return texture;
	}

	void c_post_processing::generate_geometry(const rect_t<float>& geometry_region, const rect_t<float>& uvs_region) {
		if(!mesh) mesh = factory->instance_mesh();
		if(!geometry_command) geometry_command = c_geometry_command::instance(&mesh->geometry_buffer);

		geometry_command->clear_geometry();
		geometry_command->index_count += 6;
		geometry_command->geometry_buffer
			->add_index(0).add_index(1).add_index(2)
			.add_index(0).add_index(2).add_index(3);

		rect_t<float> uvs = uvs_region / shared::viewport;
		if(renderer->framebuffer_uvs_flipped()) {
			uvs.min.y = 1.f - uvs.min.y;
			uvs.max.y = 1.f - uvs.max.y;
		}

		color_t<float> color{ };
		geometry_command->vertex_count += 4;
		geometry_command->geometry_buffer
			->add_vertex({ geometry_region.min, uvs.min, color })
			.add_vertex({ { geometry_region.max.x, geometry_region.min.y }, { uvs.max.x, uvs.min.y }, color })
			.add_vertex({ geometry_region.max, uvs.max, color })
			.add_vertex({ { geometry_region.min.x, geometry_region.max.y }, { uvs.min.x, uvs.max.y }, color });
		mesh->compile();
	}

	void c_post_processing::draw_geometry() {
		if(!mesh || !geometry_command) return;

		state_pipeline->meshes.push(mesh);
		geometry_command->handle();
		state_pipeline->meshes.pop();
	}

	void c_post_processing::generate_viewport_geometry() {
		const rect_t<float> viewport(vec2_t<float>(0), shared::viewport);
		generate_geometry(viewport, viewport);
	}

	void c_post_processing::on_create() {
		if(!mesh) generate_viewport_geometry();
	}

	void c_post_processing::on_viewport_resize_end() {
		generate_viewport_geometry();
	}
}