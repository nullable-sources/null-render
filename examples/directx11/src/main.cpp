#pragma comment(lib, "dwmapi.lib")
#include <dwmapi.h>

#include <iostream>
#include <null-render-backend-directx11.h>
#include <null-render.h>
null::render::directx11::c_window window{ };
utils::c_immediate_time_measurement timemes{ };
utils::c_cumulative_time_measurement frame_counter{ 60 };
std::shared_ptr<null::render::c_draw_list> compiled_draw_list{ };
std::shared_ptr<null::render::c_brush> brush{ null::render::c_brush::instance() };

void draw_example(const std::shared_ptr<null::render::c_draw_list>& draw_list, const std::string_view& name, const float& y) {
	//static const null::render::e_text_align text_align{ null::render::e_text_align::right | null::render::e_text_align::center_y };
	static const null::render::rounding_t rect_path_rounding{ 20.f, null::render::e_corner_flags::right_diagonal };



	/*null::render::stroke_t stroke{ };
	stroke.set_cap(null::render::e_line_cap::joint);
	stroke.set_origin(0.f);

	null::render::sdf_brush_t text_brush{ };
	text_brush
		.set_align(text_align)
		.set_size(30.f);
	null::render::background.add_text(name, { 280, y + 50 }, text_brush);*/

	null::render::pen_t pen{ };
	auto brush_pen = null::render::c_brush::instance();
	brush_pen->set_color(color_t<int>(255, 140, 140));
	pen.set_brush(brush_pen);

	auto brush_bg = null::render::c_brush::instance(color_t<int>(13, 13, 13, 255));

	//draw_list->add_command(null::render::c_matrix_command::instance(matrix4x4_t::projection_ortho(0.f, null::render::shared::viewport.x + 200, null::render::shared::viewport.y + 200, 0.f, -10000.f, 10000.f)));

	draw_list->add_convex_shape(
		null::render::make_rect({ 290 - 20, y - 20 }, { 390 + 200, y + 100 + 400 }, rect_path_rounding),
		brush_bg
	);

	draw_list->add_convex_shape(
		null::render::make_rect({ 290, y }, { 390, y + 100 }, rect_path_rounding),
		brush,
		pen
	);

	//draw_list->add_command(null::render::c_matrix_command::instance(matrix4x4_t::projection_ortho(0.f, null::render::shared::viewport.x + 200, null::render::shared::viewport.y + 200, 0.f, -10000.f, 10000.f)));

	draw_list->add_convex_shape(
		null::render::make_rect({ 290, y + 110 }, { 390, y + 210 }, rect_path_rounding),
		brush,
		pen
	);

	//draw_list->add_command(null::render::c_matrix_command::instance(null::render::backend::renderer->get_projection_matrix()));


	draw_list->add_convex_shape(
		null::render::make_rect({ 290, y + 500 }, { 390, y + 600 }, rect_path_rounding),
		brush,
		pen
	);

	/*null::render::background.add_poly_line(
		null::render::path::make_rect({ 410, y }, { 510, y + 100 }, rect_path_rounding),
		stroke,
		brush,
		pen
	);*/

	std::shared_ptr<null::render::c_filter_brush> filter_brsuh{ };
	{
		//auto filter = null::render::c_radial_gradient_filter::instance();
		//filter->set_radius(0.2f);
		//filter->set_origin(vec2_t<float>(0.5f, 0.5f));
		//filter->add_stop(color_t<int>(140, 140, 255), 0.f);
		///*filter->add_stop(color_t<int>(140, 140, 255), 0.5f);
		//filter->add_stop(color_t<int>(255, 140, 140), 0.5f);*/
		//filter->add_stop(color_t<int>(255, 140, 140), 1.f);
		//filter_brsuh = null::render::c_filter_brush::instance(filter);

		//auto filter = null::render::c_linear_gradient_filter::instance();
		////filter->set_angle(angle_t<degrees_t>(45.f));
		//filter->set_angle(angle_t<degrees_t>(360.f * std::abs(std::sin(timemes.representation().count() * 0.0000000001f))));
		//filter->set_origin({ 0.5f });
		//filter->add_stop(color_t<int>(140, 140, 255), 0.f);
		//filter->add_stop(color_t<int>(140, 140, 255), 0.5f);
		//filter->add_stop(color_t<int>(255, 140, 140), 0.5f);
		//filter->add_stop(color_t<int>(255, 140, 140), 1.f);
		//filter_brsuh = null::render::c_filter_brush::instance(filter);

		auto filter = null::render::c_blur_filter::instance();
		//filter->set_region({ vec2_t<float>(0, 0), null::render::shared::viewport });
		/*filter->downsample_count = 7;*/
		filter->downsample_count = 4;
		filter->strength = 0;
		filter->sigma = 0;
		//filter->set_radius(8);

		filter_brsuh = null::render::c_filter_brush::instance(filter);
		//filter_brsuh->set_color(color_t<int>(140, 140, 255));

		//filter_brsuh->set_color(color_t<int>(0, 0, 0));
	}

	//draw_list->add_convex_shape(
	//	null::render::make_circle({ 580 - float(400 * std::abs(std::sin(timemes.representation().count() * 0.000000001f))), y + 50 },
	//		180 + float(40 * std::abs(std::sin(timemes.representation().count() * 0.000000001f)))
	//	),
	//	filter_brsuh/*,
	//	pen*/
	//);
	auto c = null::render::shared::viewport * vec2_t<float>{ 0.27f, 0.5f };
	//auto c = null::render::shared::viewport * vec2_t<float>{ 0.37f, 0.62f };

	//auto r = null::render::shared::viewport.y * 0.1f + 20 + 1;
	auto r = null::render::shared::viewport.y * 0.5f * (1.2f * std::abs(std::sin(timemes.representation().count() * 0.0000000001f)));
	draw_list->add_convex_shape(
		null::render::make_circle(c, r),
		filter_brsuh
	);


	/*draw_list->add_convex_shape(
		null::render::make_rect(c - r, c + r),
		filter_brsuh
	);*/


	/*null::render::background.add_poly_line(
		null::render::path::make_circle({ 700, y + 50 }, 50),
		stroke,
		brush,
		pen
	);*/

	//draw_list->add_command(null::render::c_matrix_command::instance(null::render::backend::renderer->get_projection_matrix()));

}

void main_loop() {
	/*null::render::quad_gradient_filter_t quad_gradient_filter{ };
	quad_gradient_filter
		.set_top_left_color({ 255, 255, 255 })
		.set_top_right_color({ 255, 100, 0 })
		.set_bottom_left_color({ 255, 0, 100 })
		.set_bottom_right_color({ 100, 100, 255 });*/

		/*
		auto text_brush = null::render::text_brush_t::make();
		text_brush->set_filter(null::render::sdf_filter_t::make());
		*/
		/*null::render::filter_brush_t quad_gradient_brush{ };
		quad_gradient_brush.set_filter(quad_gradient_filter);*/

		/*null::render::pen_t pen_brush{ };
		pen_brush.set_layer(null::render::e_pen_layer::background);
		pen_brush.set_brush(brush);*/

		/*null::render::pen_t pen_gradient{ };
		pen_gradient.set_layer(null::render::e_pen_layer::background);
		pen_gradient.set_brush(quad_gradient_brush);*/

	null::render::begin_frame(); {
		static std::once_flag once{ };
		std::call_once(once, []() {
			compiled_draw_list = null::render::c_draw_list::instance(null::render::backend::factory->instance_mesh());
			draw_example(compiled_draw_list, "brush", 10);
			compiled_draw_list->compile();
		});

		//null::render::draw_list->add_command(null::render::c_translation_command::instance(vec2_t<float>(0, 0)));
		//null::render::draw_list->add_command(null::render::c_translation_command::instance(vec2_t<float>(100, 0)));

		auto text_brush = null::render::c_sdf_brush::instance();
		text_brush->set_size(30.f);
		text_brush->set_outline(1.f, { 100, 100, 255 }, { 100, 100, 255, 0 });
		null::render::draw_list->add_text(std::format("[ opengl ] fps: {:3.0f}, {}", 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count(), std::chrono::duration_cast<std::chrono::milliseconds>(frame_counter.representation())), { }, text_brush);

		draw_example(null::render::draw_list, "brush", 80);
		/*draw_example("brush", 10);*/
		//draw_example("brush\ngradient pen", brush, 150, pen_gradient);
		/*draw_example("gradient brush", quad_gradient_brush, 290, { });
		draw_example("gradient brush\npen", quad_gradient_brush, 430, pen_brush);*/
	} null::render::end_frame();

	null::render::backend::renderer->begin_render();
	null::render::backend::renderer->end_render();
	int i = 0;
}

int main(HINSTANCE instance) {
	//null::render::shared::msaa_quality = 1;
	window = null::render::directx11::c_window{ instance };

	window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);

	try {
		null::render::c_font::config_t config{
			.glyph_config{.ranges{ null::render::c_font::glyph_t::ranges_cyrillic() } },
			.render_mode_type = null::render::e_render_mode_type::sdf
		};

		null::render::atlas.font_loader = std::make_unique<null::render::c_truetype_loader>();
		null::render::atlas.add_font_default(&config);
		brush->set_color({ 100, 100, 255 });

		window.create();

		HRGN region{ CreateRectRgn(0, 0, -1, -1) };
		DWM_BLURBEHIND blur_behind{
			.dwFlags{ DWM_BB_ENABLE | DWM_BB_BLURREGION },
			.fEnable{ TRUE },
			.hRgnBlur{ region }
		};
		DwmEnableBlurBehindWindow(window.wnd_handle, &blur_behind);
		DeleteObject(region);

		window.main_loop();
		window.destroy();
	} catch(const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
}