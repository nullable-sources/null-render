﻿#define null_renderer_use_glfw
#define null_renderer_save_state
#include <null-render-backend-opengl3.h>
#include <null-render.h>

null::render::opengl3::c_window window{ };
utils::c_cumulative_time_measurement frame_counter{ 60 };

void draw_example(const std::string_view& name, const std::shared_ptr<null::render::c_brush>& brush, const float& y, const null::render::pen_t& pen) {
    static const null::render::e_text_align text_align{ null::render::e_text_align::right | null::render::e_text_align::center_y };
    static const null::render::rounding_t rect_path_rounding{ 20.f, null::render::e_corner_flags::right_diagonal };

    null::render::stroke_t stroke{ };
    stroke.set_cap(null::render::e_line_cap::joint);
    stroke.set_origin(0.f);

    std::shared_ptr<null::render::c_sdf_brush> text_brush = null::render::c_sdf_brush::instance();
    text_brush->set_align(text_align);
    text_brush->set_size(30.f);
    null::render::draw_list->add_text(name, { 280, y + 50 }, text_brush);

    null::render::draw_list->add_convex_shape(
        null::render::make_rect({ 290, y }, { 390, y + 100 }, rect_path_rounding),
        brush,
        pen
    );

    null::render::draw_list->add_poly_line(
        null::render::make_rect({ 410, y }, { 510, y + 100 }, rect_path_rounding),
        stroke,
        brush,
        pen
    );

    null::render::draw_list->add_convex_shape(
        null::render::make_circle({ 580, y + 50 }, 50),
        brush,
        pen
    );

    null::render::draw_list->add_poly_line(
        null::render::make_circle({ 700, y + 50 }, 50),
        stroke,
        brush,
        pen
    );
}

void main_loop() {
    std::shared_ptr<null::render::c_quad_gradient_filter> quad_gradient_filter = null::render::c_quad_gradient_filter::instance();
    quad_gradient_filter->set_top_left_color({ 255, 255, 255 });
    quad_gradient_filter->set_top_right_color({ 255, 100, 0 });
    quad_gradient_filter->set_bottom_left_color({ 255, 0, 100 });
    quad_gradient_filter->set_bottom_right_color({ 100, 100, 255 });

    std::shared_ptr<null::render::c_brush> brush = null::render::c_brush::instance();
    brush->set_color({ 100, 100, 255 });

    std::shared_ptr<null::render::c_filter_brush> quad_gradient_brush = null::render::c_filter_brush::instance();
    quad_gradient_brush->set_filter(quad_gradient_filter);

    null::render::pen_t pen_brush{ };
    pen_brush.set_layer(null::render::e_pen_layer::background);
    pen_brush.set_brush(brush);

    null::render::pen_t pen_gradient{ };
    pen_gradient.set_layer(null::render::e_pen_layer::background);
    pen_gradient.set_brush(quad_gradient_brush);

    null::render::begin_frame(); {
        std::shared_ptr<null::render::c_sdf_brush> text_brush = null::render::c_sdf_brush::instance();
        text_brush->set_size(30.f);
        text_brush->set_outline(1.f, { 100, 100, 255 }, { 100, 100, 255, 0 });
        null::render::draw_list->add_text(std::format("[ opengl3 ] fps: {:3.0f}", 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count()), { }, text_brush);

        draw_example("brush", brush, 10, { });
        draw_example("brush\ngradient pen", brush, 150, pen_gradient);
        draw_example("gradient brush", quad_gradient_brush, 290, { });
        draw_example("gradient brush\npen", quad_gradient_brush, 430, pen_brush);
    } null::render::end_frame();

    null::render::backend::renderer->begin_render();
    null::render::backend::renderer->end_render();
}

int main() {
    window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
    window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });
    window.clear_color = color_t<float>(0.f, 0.f, 0.f, 0.f);
    window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);
    //main_loop();
    try {
        null::render::c_font::config_t config{
            .glyph_config{ .ranges{ null::render::c_font::glyph_t::ranges_cyrillic() } },
            .render_mode_type = null::render::e_render_mode_type::sdf
        };

        null::render::atlas.font_loader = std::make_unique<null::render::c_truetype_loader>();
        null::render::atlas.add_font_default(&config);

        window.create();
        window.main_loop();
        window.destroy();
    } catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl;
    }
}