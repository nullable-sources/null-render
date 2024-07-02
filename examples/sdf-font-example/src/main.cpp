#define gl

#ifdef dx9
#include <null-render-backend-directx9.h>
#elif defined dx11
#include <null-render-backend-directx11.h>
#elif defined gl
#define null_renderer_use_glfw
#define null_renderer_save_state
#include <null-render-backend-opengl3.h>
#endif
#include <null-render.h>

#ifdef dx9
null::render::directx9::c_window window{ };
#elif defined dx11
null::render::directx11::c_window window{ };
#elif defined gl
null::render::opengl3::c_window window{ };
#endif
utils::c_cumulative_time_measurement frame_counter{ 60 };

void draw_font(std::shared_ptr<null::render::c_mtsdf_brush>& brush, std::string_view render_mode, std::string_view name, float size, vec2_t<float>& offset) {
    brush->set_size(size);
    float size_x = null::render::draw_list->add_text(std::format("[{}] {}: {}", render_mode, name, size), { }, brush).x;
    offset.x += size_x + 10.f;
    null::render::draw_list->add_command(null::render::c_update_translation_command::instance(offset));
}

void draw_fonts(std::string_view render_mode, const vec2_t<float>& src_offset) {
    std::shared_ptr<null::render::c_mtsdf_brush> text_brush = null::render::c_mtsdf_brush::instance();
    text_brush->set_outline_color(color_t<int>(0, 255));
    text_brush->set_outline_width(0.3, true);

    vec2_t<float> offset = src_offset;
    null::render::draw_list->add_command(null::render::c_update_translation_command::instance(offset));

    for(int size : std::views::iota(25, 27) | std::views::reverse)
        draw_font(text_brush, render_mode, "b", size, offset);
    for(int size : std::views::iota(8, 14) | std::views::reverse)
        draw_font(text_brush, render_mode, "b", size, offset);

    null::render::draw_list->add_command(null::render::c_update_translation_command::instance({ 0.f, 0 }));
}

void main_loop() {
    null::render::begin_frame(); {
        draw_fonts("mtsdf", vec2_t<float>(10, 50));
    } null::render::end_frame();

    null::render::backend::renderer->begin_render();
    null::render::backend::renderer->end_render();
}

int main(HINSTANCE instance) {
#ifdef dx9
    window = null::render::directx9::c_window{ instance };
#elif defined dx11
    window = null::render::directx11::c_window{ instance };
#elif defined gl
    window = null::render::opengl3::c_window{ instance };
#endif
    window.clear_color = color_t<int>{ 40, 40, 40 };

    window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
    window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

    window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);

    try { 
        null::render::atlas.font_loader = std::make_unique<null::render::c_freetype_loader>();

        //@note: .arfont generated with "-size 40 -pxrange 10 -pxalign off -type mtsdf -format binfloat" args
        null::render::font_config_t config{ };
        config.load_font_from_resource("roboto.ttf", "fonts")
              .load_artery_from_resource("roboto-mtsdf.arfont", "fonts")
              .set_render_mode(null::render::e_font_render_mode::mtsdf)
              .set_size(40.f)
              .set_pixel_range(10.f);
        null::render::atlas.add_font(config);

        window.create();
        window.main_loop();
        window.destroy();
    } catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl;
    }
}