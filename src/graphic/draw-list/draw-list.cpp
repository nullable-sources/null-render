#include <backend/internal/mesh.h>
#include <graphic/commands/geometry-command/geometry-command.h>

#include <graphic/draw-list/draw-list.h>

namespace null::render {
	void c_draw_list::add_rect(const vec2_t<float>& a, const vec2_t<float>& b, const brush_t& brush) {
		std::unique_ptr<commands::c_geometry> command{ std::make_unique<commands::c_geometry>() };

		command->index_count += 6;
		backend::mesh->geometry_buffer
			.add_index(0).add_index(1).add_index(2)
			.add_index(0).add_index(2).add_index(3);

		command->vertex_count += 4;
		backend::mesh->geometry_buffer
			.add_vertex({ a, { 0.f }, brush.color })
			.add_vertex({ { b.x, a.y }, { 1.f, 0.f }, brush.color })
			.add_vertex({ b, { 1.f }, brush.color })
			.add_vertex({ { a.x, b.y }, { 0.f, 1.f }, brush.color });

		add_command(brush.prepare_command(command));
	}
	
	void c_draw_list::add_quad(const rect_t<float>& a, const rect_t<float>& b, const brush_t& brush) {
		std::unique_ptr<commands::c_geometry> command{ std::make_unique<commands::c_geometry>() };

		command->index_count += 6;
		backend::mesh->geometry_buffer
			.add_index(0).add_index(1).add_index(2)
			.add_index(0).add_index(2).add_index(3);

		command->vertex_count += 4;
		backend::mesh->geometry_buffer
			.add_vertex({ a.min, { 0.f }, brush.color })
			.add_vertex({ a.max, { 1.f, 0.f }, brush.color })
			.add_vertex({ b.max, { 1.f }, brush.color })
			.add_vertex({ b.min, { 0.f, 1.f }, brush.color });

		add_command(brush.prepare_command(command));
	}

	void c_draw_list::add_text(const std::string_view& text, const vec2_t<float>& _pos, const text_style_t& text_style) {
		std::unique_ptr<commands::c_geometry> command{ std::make_unique<commands::c_geometry>() };
		vec2_t<float> pos{ _pos };
		pos = math::floor(pos);
		for(auto iterator{ text.begin() }; iterator != text.end();) {
			std::uint32_t symbol{ (std::uint32_t)*iterator };
			iterator += impl::char_converters::converter<char>::convert(symbol, iterator, text.end());
			if(!symbol) break;

			if(symbol == '\r') continue;
			if(symbol == '\n') {
				pos.x = _pos.x;
				pos.y += text_style.size;
				continue;
			}

			const c_font::glyph_t* glyph{ text_style.font->find_glyph((std::uint16_t)symbol) };
			if(!glyph) continue;

			if(glyph->visible) {
				rect_t corners{ rect_t{ pos } + glyph->corners * (text_style.size / text_style.font->size) };
				rect_t uvs{ glyph->texture_coordinates };

				command->index_count += 6;
				backend::mesh->geometry_buffer
					.add_index(command->vertex_count).add_index(command->vertex_count + 1).add_index(command->vertex_count + 2)
					.add_index(command->vertex_count).add_index(command->vertex_count + 2).add_index(command->vertex_count + 3);

				command->vertex_count += 4;
				backend::mesh->geometry_buffer
					.add_vertex({ corners.min, uvs.min, text_style.color })
					.add_vertex({ { corners.max.x, corners.min.y }, { uvs.max.x, uvs.min.y }, text_style.color })
					.add_vertex({ corners.max, uvs.max, text_style.color })
					.add_vertex({ { corners.min.x, corners.max.y }, { uvs.min.x, uvs.max.y }, text_style.color });
			}
			pos.x += glyph->advance_x * (text_style.size / text_style.font->size);
		}

		add_command(text_style.prepare_command(command));
	}
}