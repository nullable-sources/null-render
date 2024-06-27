#include <null-render-backend-directx9.h>
#include <null-render.h>

null::render::directx9::c_window window{ };
utils::c_cumulative_time_measurement frame_counter{ 60 };

struct fonts_t {
public:
    null::render::c_font* big_font{ }, *medium_font{ }, *small_font{ };
} truetype{ };

void draw_font(std::shared_ptr<null::render::c_sdf_brush>& brush, std::string_view loader, std::string_view name, float size, vec2_t<float>& offset) {
    brush->set_size(size);
    float size_x = null::render::draw_list->add_text(std::format("[{}] {}: {}", loader, name, size), { }, brush).x;
    offset.x += size_x + 10.f;
    null::render::draw_list->add_command(null::render::c_update_translation_command::instance(offset));
}

void draw_fonts(std::string_view loader, fonts_t& fonts, const vec2_t<float>& src_offset) {
    std::shared_ptr<null::render::c_sdf_brush> text_brush = null::render::c_sdf_brush::instance();
    text_brush->set_outline(1.f, color_t<int>::palette_t::black, color_t<int>{ 0, 255 });

    vec2_t<float> offset = src_offset;
    null::render::draw_list->add_command(null::render::c_update_translation_command::instance(offset));

    text_brush->set_font(fonts.big_font);
    for(int size : std::views::iota(25, 28) | std::views::reverse)
        draw_font(text_brush, loader, "b", size, offset);
    for(int size : std::views::iota(8, 15) | std::views::reverse)
        draw_font(text_brush, loader, "b", size, offset);
    offset = vec2_t<float>(src_offset.x, offset.y + 40);
    null::render::draw_list->add_command(null::render::c_update_translation_command::instance(offset));

    text_brush->set_font(fonts.medium_font);
    for(int size : std::views::iota(25, 28) | std::views::reverse)
        draw_font(text_brush, loader, "m", size, offset);
    for(int size : std::views::iota(8, 15) | std::views::reverse)
        draw_font(text_brush, loader, "m", size, offset);
    offset = vec2_t<float>(src_offset.x, offset.y + 40);
    null::render::draw_list->add_command(null::render::c_update_translation_command::instance(offset));

    text_brush->set_font(fonts.small_font);
    for(int size : std::views::iota(25, 28) | std::views::reverse)
        draw_font(text_brush, loader, "s", size, offset);
    for(int size : std::views::iota(8, 15) | std::views::reverse)
        draw_font(text_brush, loader, "s", size, offset);

    null::render::draw_list->add_command(null::render::c_update_translation_command::instance({ 0.f, 0 }));
}

void main_loop() {
    null::render::begin_frame(); {
        std::shared_ptr<null::render::c_sdf_brush> text_brush = null::render::c_sdf_brush::instance();
        text_brush->set_size(30.f);
        text_brush->set_outline(1.f, { 100, 100, 255 }, { 100, 100, 255, 0 });
        null::render::draw_list->add_text(std::format("[ directx9 ] fps: {:3.0f}", 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count()), { }, text_brush);

        draw_fonts("tt", truetype, vec2_t<float>(10, 50));
    } null::render::end_frame();

    null::render::backend::renderer->begin_render();
    null::render::backend::renderer->end_render();
}

int main(HINSTANCE instance) {
    window = null::render::directx9::c_window{ instance };
    window.clear_color = color_t<int>{ 180, 180, 180 };

    window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
    window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

    window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);

    try {
        null::render::c_font::config_t config{
            .glyph_config{ .ranges{ null::render::c_font::glyph_t::ranges_cyrillic() } },
            .render_mode_type = null::render::e_render_mode_type::sdf
        };

        null::render::atlas.font_loader = std::make_unique<null::render::c_truetype_loader>();
        
        config.size_pixels = 36.f;
        truetype.big_font = null::render::atlas.add_font_default(&config);

        config.size_pixels = 18.f;
        truetype.medium_font = null::render::atlas.add_font_default(&config);

        config.size_pixels = 12.f;
        truetype.small_font = null::render::atlas.add_font_default(&config);

        window.create();
        window.main_loop();
        window.destroy();
    } catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl;
    }
}