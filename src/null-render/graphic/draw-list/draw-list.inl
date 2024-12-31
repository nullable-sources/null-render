#include "draw-list.h"

namespace ntl::render {
    template <typename char_t>
    std::shared_ptr<i_command> c_draw_list::make_text_command(std::basic_string_view<char_t> text, vec2_t<float> pos, const std::shared_ptr<i_text_brush>& text_brush, vec2_t<float>* out_size) {
        const color_t<int>& brush_color = text_brush->color;
        if(brush_color.a <= 0) return nullptr;
        std::shared_ptr<c_geometry_command> command = c_geometry_command::instance(&mesh->geometry_buffer);

        c_font* font = text_brush->font ? text_brush->font : get_default_font();
        const float scale = text_brush->size == -1.f ? 1.f : text_brush->size / font->metrics.size;
        const float line_spacing = text_brush->line_spacing * scale;
        const float letter_spacing = text_brush->letter_spacing * scale;
        const float line_height = font->metrics.line_height * scale + line_spacing;

        pos = math::floor(pos);
        float max_line_width{ };

        if(out_size) *out_size = pos;
        const float carriage_return = pos.x;
        std::uint32_t previous_symbol{ };
        for(auto iterator = text.begin(); iterator != text.end();) {
            std::uint32_t symbol = (std::uint32_t)*iterator;
            iterator += impl::char_converters::converter<char_t>::convert(symbol, iterator, text.end());
            if(!symbol) break;

            if(symbol == '\r') continue;
            if(symbol == '\n') {
                if(out_size) max_line_width = std::max(max_line_width, pos.x);
                pos.x = carriage_return;
                pos.y += line_height;
                continue;
            }

            const glyph_t* glyph = font->find_glyph(symbol);
            if(!glyph) continue;

            pos.x += font->lookup_kerning(previous_symbol, symbol) * scale;

            if(glyph->visible) {
                const rect_t<float> corners = glyph->rectangle * scale + pos;
                const rect_t<float>& upper_uvs = glyph->upper_texture_uvs;
                const rect_t<float>& lower_uvs = glyph->lower_texture_uvs;

                command->index_count += 6;
                mesh->geometry_buffer
                    .add_indexes(
                        command->vertex_count, command->vertex_count + 1, command->vertex_count + 2,
                        command->vertex_count, command->vertex_count + 2, command->vertex_count + 3
                    );

                command->vertex_count += 4;
                mesh->geometry_buffer
                    .add_vertex(corners.min, upper_uvs.min, brush_color)
                    .add_vertex(vec2_t<float>(corners.max.x, corners.min.y), upper_uvs.max, brush_color)
                    .add_vertex(corners.max, lower_uvs.max, brush_color)
                    .add_vertex(vec2_t<float>(corners.min.x, corners.max.y), lower_uvs.min, brush_color);
            }
            pos.x += glyph->advance * scale + letter_spacing;

            previous_symbol = symbol;
        }
        pos.y += line_height;

        if(out_size) *out_size = vec2_t<float>(std::max(max_line_width, pos.x), pos.y) - *out_size;
        return text_brush->prepare_command(std::move(command));
    }

    template <typename char_t>
    vec2_t<float> c_draw_list::add_text(std::basic_string_view<char_t> text, vec2_t<float> pos, const std::shared_ptr<i_text_brush>& text_brush) {
        vec2_t<float> str_size{ };
        std::shared_ptr<i_command> command = make_text_command(text, pos, text_brush, &str_size);
        if(!command) return { };

        if(text_brush->align != e_text_align::none) {
            vec2_t<float> translate{ };

            if(text_brush->align & e_text_align::right) translate.x = str_size.x;
            if(text_brush->align & e_text_align::bottom) translate.y = str_size.y;
            if(text_brush->align & e_text_align::center_x) translate.x = str_size.x / 2.f;
            if(text_brush->align & e_text_align::center_y) translate.y = str_size.y / 2.f;
            add_command(c_update_translation_command::instance(-translate));
        }

        add_command(std::move(command));
        if(text_brush->align != e_text_align::none)
            add_command(c_update_translation_command::instance(0.f));

        return str_size;
    }
}