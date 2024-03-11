#include "../backend.h"

namespace null::render::backend {
	void c_post_processing::initialize() {
		for(auto& buffer : post_processing->frame_buffers) {
			buffer = factory->instance_frame_buffer(shared::viewport, e_frame_buffer_type::postprocessing, backend::e_frame_buffer_flags::none);
		}

		mesh = factory->instance_mesh();
		transfer_buffer = factory->instance_frame_buffer(shared::viewport, e_frame_buffer_type::postprocessing, e_frame_buffer_flags::none);
	}

	rect_t<float> c_post_processing::prepare_viewport_region(const rect_t<float>& screen_region) {
		return rect_t<float>(
			vec4_t<float>(viewport_matrix * vec4_t<float>(screen_region.min, 0.f, 1.f)).xy(),
			vec4_t<float>(viewport_matrix * vec4_t<float>(screen_region.max, 0.f, 1.f)).xy()
		);
	}

	void c_post_processing::generate_draw_geometry(c_geometry_command* command, const rect_t<float>& geometry_region, rect_t<float> uvs) {
		command->index_count += 6;
		command->geometry_buffer
			->add_index(0).add_index(1).add_index(2)
			.add_index(0).add_index(2).add_index(3);

		if(renderer->framebuffer_uvs_flipped()) {
			uvs.min.y = 1.f - uvs.min.y;
			uvs.max.y = 1.f - uvs.max.y;
		}

		color_t<float> color{ };
		command->vertex_count += 4;
		command->geometry_buffer
			->add_vertex({ geometry_region.min, uvs.min, color })
			.add_vertex({ { geometry_region.max.x, geometry_region.min.y }, { uvs.max.x, uvs.min.y }, color })
			.add_vertex({ geometry_region.max, uvs.max, color })
			.add_vertex({ { geometry_region.min.x, geometry_region.max.y }, { uvs.min.x, uvs.max.y }, color });
	}

	void c_post_processing::blit_buffer_region(i_frame_buffer* buffer, const vec2_t<float>& uvs) {
		state_pipeline->shaders.push(passthrough_shader);
		draw_buffer_region(buffer, uvs);
		state_pipeline->shaders.pop();
	}

	void c_post_processing::blit_buffer_region(i_frame_buffer* buffer, const rect_t<float>& geometry_region, const rect_t<float>& uvs_region) {
		state_pipeline->shaders.push(passthrough_shader);
		draw_buffer_region(buffer, geometry_region, uvs_region);
		state_pipeline->shaders.pop();
	}

	void c_post_processing::blit_buffer(i_frame_buffer* buffer) {
		state_pipeline->shaders.push(passthrough_shader);
		draw_buffer(buffer);
		state_pipeline->shaders.pop();
	}

	void c_post_processing::draw_buffer_region(i_frame_buffer* buffer, const vec2_t<float>& uvs) {
		generate_geometry(uvs);
		draw_buffer_texture(buffer);
	}

	void c_post_processing::draw_buffer_region(i_frame_buffer* buffer, const rect_t<float>& geometry_region, const rect_t<float>& uvs_region) {
		generate_geometry(prepare_viewport_region(geometry_region), uvs_region / shared::viewport);
		draw_buffer_texture(buffer);
	}

	void c_post_processing::draw_buffer(i_frame_buffer* buffer) {
		if(geometry_dirty) generate_viewport_geometry();
		draw_buffer_texture(buffer);
	}

	void c_post_processing::generate_geometry(const vec2_t<float>& uv_scaling) {
		generate_geometry(normalized_viewport, normalized_uvs * uv_scaling);
	}

	void c_post_processing::generate_geometry(const rect_t<float>& geometry_region, const rect_t<float>& uvs_region) {
		if(!mesh) mesh = factory->instance_mesh();
		if(!geometry_command) geometry_command = c_geometry_command::instance(&mesh->geometry_buffer);

		geometry_command->clear_geometry();
		generate_draw_geometry(geometry_command.get(), geometry_region, uvs_region);
		mesh->compile();

		geometry_dirty = true;
	}

	void c_post_processing::draw_buffer_texture(i_frame_buffer* buffer) {
		if(!mesh || !geometry_command) return;

		state_pipeline->textures.push(prepare_buffer_texture(buffer));
		state_pipeline->meshes.push(mesh);
		geometry_command->handle();
		state_pipeline->meshes.pop();
		state_pipeline->textures.pop();
	}

	void* c_post_processing::prepare_buffer_texture(i_frame_buffer* buffer) {
		void* texture = buffer->get_texture();
		if(buffer->flags & e_frame_buffer_flags::msaa) {
			transfer_buffer->clear();
			transfer_buffer->copy_from(buffer);
			texture = transfer_buffer->get_texture();
		}

		return texture;
	}

	void c_post_processing::generate_viewport_geometry() {
		viewport_matrix = renderer->get_projection_matrix();

		generate_geometry(normalized_viewport, normalized_uvs);

		geometry_dirty = false;
	}

	void c_post_processing::on_create() {
		if(!geometry_command) generate_viewport_geometry();
	}

	void c_post_processing::on_viewport_resize_end() {
		generate_viewport_geometry();
	}
}