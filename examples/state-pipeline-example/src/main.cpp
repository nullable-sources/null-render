#define dx11

#ifdef dx9
#include <null-render-backend-directx9.h>
#elif defined dx11
#include <null-render-backend-directx11.h>
#elif defined gl
#define null_renderer_use_glfw
#include <null-render-backend-opengl3.h>
#endif

#include <null-render.h>

#ifdef dx9
ntl::render::directx9::c_window window{ };
#elif defined dx11
ntl::render::directx11::c_window window{ };
#elif defined gl
ntl::render::opengl3::c_window window{ };
#endif

#ifdef dx9
static constexpr std::string_view backend_name = "directx9";
#elif defined dx11
static constexpr std::string_view backend_name = "directx11";
#elif defined gl
static constexpr std::string_view backend_name = "opengl3";
#endif

ntl::c_immediate_time_measurement time_counter{ };
ntl::c_cumulative_time_measurement frame_counter{ 60 };
std::unique_ptr<ntl::render::backend::i_blend_state> blend_state{ }, blend_state_second{ };
std::unique_ptr<ntl::render::backend::i_rasterizer_state> rasterizer_state{ };

void draw_example(const std::string_view& name, const std::shared_ptr<ntl::render::c_brush>& brush, const float& y, const ntl::render::pen_t& pen) {
    static const ntl::render::e_text_align text_align{ ntl::render::e_text_align::right | ntl::render::e_text_align::center_y };
    static const ntl::render::rounding_t rect_path_rounding{ 20.f, ntl::render::e_corner_flags::right_diagonal };

    ntl::render::stroke_t stroke{ };
    stroke.set_cap(ntl::render::e_line_cap::joint);
    stroke.set_origin(0.f);

    std::shared_ptr<ntl::render::c_sdf_brush> text_brush = ntl::render::c_sdf_brush::instance();
    text_brush->set_align(text_align);
    text_brush->set_size(30.f);
    ntl::render::draw_list->add_text(name, { 280, y + 50 }, text_brush);

    ntl::render::draw_list->add_convex_shape(
        ntl::render::make_rect({ 290, y }, { 390, y + 100 }, rect_path_rounding),
        brush,
        pen
    );

    ntl::render::draw_list->add_poly_line(
        ntl::render::make_rect({ 410, y }, { 510, y + 100 }, rect_path_rounding),
        stroke,
        brush,
        pen
    );

    ntl::render::draw_list->add_convex_shape(
        ntl::render::make_circle({ 580, y + 50 }, 50),
        brush,
        pen
    );

    ntl::render::draw_list->add_poly_line(
        ntl::render::make_circle({ 700, y + 50 }, 50),
        stroke,
        brush,
        pen
    );
}

void main_loop() {
    std::shared_ptr<ntl::render::c_quad_gradient_filter> quad_gradient_filter = ntl::render::c_quad_gradient_filter::instance();
    quad_gradient_filter->set_top_left_color({ 255, 255, 255 });
    quad_gradient_filter->set_top_right_color({ 255, 100, 0 });
    quad_gradient_filter->set_bottom_left_color({ 255, 0, 100 });
    quad_gradient_filter->set_bottom_right_color({ 100, 100, 255 });

    std::shared_ptr<ntl::render::c_brush> brush = ntl::render::c_brush::instance();
    brush->set_color({ 100, 100, 255 });

    std::shared_ptr<ntl::render::c_filter_brush> quad_gradient_brush = ntl::render::c_filter_brush::instance();
    quad_gradient_brush->set_filter(quad_gradient_filter);

    ntl::render::pen_t pen_brush{ };
    pen_brush.set_layer(ntl::render::e_pen_layer::background);
    pen_brush.set_brush(brush);

    ntl::render::pen_t pen_gradient{ };
    pen_gradient.set_layer(ntl::render::e_pen_layer::background);
    pen_gradient.set_brush(quad_gradient_brush);

    ntl::render::begin_frame(); {
        std::shared_ptr<ntl::render::c_sdf_brush> text_brush = ntl::render::c_sdf_brush::instance();
        text_brush->set_size(30.f);
        text_brush->set_outline_blur(1.f);
        text_brush->set_outline_color(color_t<int>(100, 100, 255));
        text_brush->set_outline_width(2.f);
        ntl::render::draw_list->add_text(std::format("[ {} ] fps: {:3.0f}", backend_name, 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count()), { }, text_brush);

        float time = std::chrono::duration_cast<std::chrono::duration<float>>(time_counter.representation()).count();
        blend_state->unlock();
        blend_state->factor.set(std::abs(std::sin(time * 0.6f)));
        blend_state->lock();

        blend_state_second->unlock();
        blend_state_second->factor.set(std::abs(std::sin(time * 0.8f)));
        blend_state_second->lock();

        ntl::render::draw_list->add_command(ntl::render::c_clip_command::instance(rect_t<float>(vec2_t<float>(0), vec2_t<float>(10))));
        draw_example("brush", brush, 10, { });
        ntl::render::draw_list->add_command(ntl::render::c_rasterizer_push_command::instance(rasterizer_state));
        ntl::render::draw_list->add_command(ntl::render::c_blend_push_command::instance(blend_state));
        draw_example("brush\ngradient pen", brush, 150, pen_gradient);
        ntl::render::draw_list->add_command(ntl::render::c_blend_push_command::instance(blend_state_second));
        draw_example("gradient brush", quad_gradient_brush, 290, { });
        ntl::render::draw_list->add_command(ntl::render::c_blend_pop_command::instance());
        ntl::render::draw_list->add_command(ntl::render::c_blend_pop_command::instance());
        ntl::render::draw_list->add_command(ntl::render::c_rasterizer_pop_command::instance());
        draw_example("gradient brush\npen", quad_gradient_brush, 430, pen_brush);
        ntl::render::draw_list->add_command(ntl::render::c_clip_command::instance(rect_t<float>(vec2_t<float>(0), ntl::render::shared::viewport)));
    } ntl::render::end_frame();

    ntl::render::backend::renderer->begin_render();
    ntl::render::backend::renderer->end_render();
}

int main(HINSTANCE instance) {
#ifdef dx9
    window = ntl::render::directx9::c_window{ instance };
#elif defined dx11
    window = ntl::render::directx11::c_window{ instance };
#elif defined gl
    window = ntl::render::opengl3::c_window{ instance };
#endif
    window.clear_color = color_t<int>{ 40, 40, 40 };

    window.callbacks.at<ntl::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); time_counter.begin(); });
    window.callbacks.at<ntl::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

    window.callbacks.at<ntl::win::e_window_callbacks::on_main_loop>().add(main_loop);

    try {
        ntl::render::font_config_t config{ };
        config.load_font_default()
            .set_render_mode(ntl::render::e_font_render_mode::sdf)
            .set_pixel_range(2.f)
            .set_size(14.f);

        ntl::render::atlas.font_loader = std::make_unique<ntl::render::c_freetype_loader>();
        ntl::render::atlas.add_font(config);

        window.create();

        rasterizer_state = ntl::render::backend::factory->instance_rasterizer_state();
        rasterizer_state->unlock();
        rasterizer_state->msaa_disable.set(true);
        rasterizer_state->scissor_disable.set(true);
        rasterizer_state->lock();

        blend_state = ntl::render::backend::factory->instance_blend_state();
        blend_state->unlock();
        blend_state->src_blend.set(ntl::render::backend::e_blend::constant_factor);
        blend_state->dst_blend.set(ntl::render::backend::e_blend::zero);
        blend_state->lock();

        blend_state_second = blend_state->clone();


        window.main_loop();
        window.destroy();
    } catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl;
    }
}