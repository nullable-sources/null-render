#include "draw-list.h"

namespace null::render {
    template <typename char_t>
    vec2_t<float> c_draw_list::add_text(std::basic_string_view<char_t> text, vec2_t<float> pos, const std::shared_ptr<i_text_brush>& text_brush) {
        if(text_brush->color.a <= 0) return { };
        std::shared_ptr<c_geometry_command> command = c_geometry_command::instance(&mesh->geometry_buffer);

        if(text_brush->align != e_text_align::none) {
            vec2_t text_size = text_brush->font->calc_text_size<char_t>(text, text_brush->size);
            if(text_size <= 0.f) return { };

            if(text_brush->align & e_text_align::right) pos.x -= text_size.x;
            if(text_brush->align & e_text_align::bottom) pos.y -= text_size.y;
            if(text_brush->align & e_text_align::center_x) pos.x -= text_size.x / 2.f;
            if(text_brush->align & e_text_align::center_y) pos.y -= text_size.y / 2.f;
        }

        float new_line_pos = pos.x;
        const vec2_t<float> src_pos = pos = math::floor(pos);
        for(auto iterator = text.begin(); iterator != text.end();) {
            std::uint32_t symbol = (std::uint32_t)*iterator;
            iterator += impl::char_converters::converter<char_t>::convert(symbol, iterator, text.end());
            if(!symbol) break;

            if(symbol == '\r') continue;
            if(symbol == '\n') {
                pos.x = new_line_pos;
                pos.y += text_brush->size;
                continue;
            }

            const c_font::glyph_t* glyph = text_brush->font->find_glyph((std::uint16_t)symbol);
            if(!glyph) continue;

            if(glyph->visible) {
                rect_t corners = rect_t(pos) + glyph->corners * (text_brush->size / text_brush->font->size);
                rect_t uvs = glyph->texture_coordinates;

                command->index_count += 6;
                mesh->geometry_buffer
                    .add_index(command->vertex_count).add_index(command->vertex_count + 1).add_index(command->vertex_count + 2)
                    .add_index(command->vertex_count).add_index(command->vertex_count + 2).add_index(command->vertex_count + 3);

                command->vertex_count += 4;
                mesh->geometry_buffer
                    .add_vertex(corners.min, uvs.min, text_brush->color)
                    .add_vertex(vec2_t(corners.max.x, corners.min.y), vec2_t(uvs.max.x, uvs.min.y), text_brush->color)
                    .add_vertex(corners.max, uvs.max, text_brush->color)
                    .add_vertex(vec2_t(corners.min.x, corners.max.y), vec2_t(uvs.min.x, uvs.max.y), text_brush->color);
            }
            pos.x += glyph->advance_x * (text_brush->size / text_brush->font->size);
        }

        add_command(text_brush->prepare_command(std::move(command)));
        return pos - src_pos;
    }
}