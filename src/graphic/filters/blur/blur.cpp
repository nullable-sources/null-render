#include <backend/renderer/renderer.h>
#include <backend/internal/frame-buffer.h>
#include <backend/shaders/passthrough/passthrough.h>
#include <backend/shaders/blur/blur.h>

#include <graphic/filters/blur/blur.h>

#include <graphic/path/path.h>

namespace null::render::filters {
	void c_blur::make_geometry() {
		geometry_pass = std::make_unique<commands::c_geometry>();

		geometry_pass->index_count += 6;
		backend::mesh->geometry_buffer
			.add_index(0).add_index(1).add_index(2)
			.add_index(0).add_index(2).add_index(3);

		rect_t<float> uvs{ region / rect_t<float>{ backend::postprocessing_buffers[0]->size } };
		geometry_pass->vertex_count += 4;
		backend::mesh->geometry_buffer
			.add_vertex({ region.min, uvs.min, { } })
			.add_vertex({ { region.min.y, region.max.x }, vec2_t{ uvs.min.x, uvs.max.y }, { } })
			.add_vertex({ region.max, uvs.max, { } })
			.add_vertex({ { region.max.y, region.min.x }, vec2_t{ uvs.max.x, uvs.min.y }, { } });
	}

	void c_blur::handle() {
		backend::postprocessing_buffers[1]->set();
		backend::postprocessing_buffers[1]->clear();

		child_command->handle();

		backend::shaders::blur->set_amount(amount);
		backend::shaders::blur->set_texel_size(vec2_t{ 1.f } / backend::postprocessing_buffers[0]->size);
		backend::shaders::blur->set_uv_limits((region + rect_t<float>{ { 0.5f }, { -0.5f } }) / rect_t<float>{ backend::postprocessing_buffers[0]->size });
		backend::shaders::blur->use();

		for(const int& i : std::views::iota(0, 8)) {
			backend::shaders::blur->set_direction({ 1.f, 0.f });
			backend::postprocessing_buffers[1]->set();
			backend::renderer->set_texture(backend::postprocessing_buffers[0]->get_texture());
			geometry_pass->handle();

			backend::shaders::blur->set_direction({ 0.f, 1.f });
			backend::postprocessing_buffers[0]->set();
			backend::renderer->set_texture(backend::postprocessing_buffers[1]->get_texture());
			geometry_pass->handle();
		}

		backend::shaders::passthrough_color->use();
		backend::renderer->set_texture(nullptr);

		backend::msaa_buffer->set();
	}
}