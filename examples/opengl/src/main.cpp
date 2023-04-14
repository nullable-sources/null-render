﻿#include <iostream>
#define null_renderer_use_glfw
#define null_renderer_save_state
#include <null-backend-opengl3.h>
#include <null-render.h>

null::render::backend::opengl3::c_window window{ };
utils::c_cumulative_time_measurement frame_counter{ 60 };

void main_loop() {
	/*static const null::render::e_text_flags text_flags{ null::render::e_text_flags::aligin_bottom | null::render::e_text_flags::aligin_center_x };
	static const null::render::e_corner_flags corner_flags{ null::render::e_corner_flags::right_diagonal };*/

	static const color_t color{ 100, 100, 255 };

	static const null::render::multicolor_text_t<std::string> multicolor_text{ {
		{ "multicolored \n\n", { } },
		{ "text", color }
	} };

	null::render::begin_frame(window); {
		null::render::brush_t brush{ };

		brush.set_color({ 255, 100, 255 });
		null::render::background.add_poly_line({ { 350, 220 }, { 500, 180}, { 500, 450 }, { 350, 400 } }, brush, null::render::stroke_t{ }.set_join(null::render::e_line_join::bevel).set_cap(null::render::e_line_cap::joint).set_thickness(50.f));

		brush.set_color({ 100, 255, 255 });
		null::render::background.add_convex_shape({ { 150, 200 }, { 300, 180}, { 300, 450 }, { 150, 400 } }, brush);

		null::render::quad_gradient_brush_t gradient_brush{ };
		gradient_brush
			.set_top_left_color({ 255, 255, 255 })
			.set_top_right_color({ 255, 100, 0 })
			.set_bottom_left_color({ 255, 0, 100 })
			.set_bottom_right_color({ 100, 100, 255 });
		null::render::background.add_rect({ 600, 200 }, { 700, 300 }, gradient_brush);

		null::render::sdf_text_style_t text_style{ };
		text_style
			.set_color({ 255, 100, 100 })
			.set_size(40.f)
			.set_outline(1.f, { 255, 200, 200 }, { 100, 100, 255 });
		null::render::background.add_text("sdf text", { }, text_style);
		
		//custom_buffer.add_text("text drawed by custom geometry buffer", { 100, 10 }, { });
		//null::render::background.add_text(std::format("[ opengl3 ] fps: {:3.3f}", 1000.f /( 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count())), { (float)window.get_window_size().x, 10.f }, { }, null::render::e_text_flags{ -null::render::e_text_flags::aligin_right | -null::render::e_text_flags::aligin_center_y | -null::render::e_text_flags::outline });
		//null::render::background.add_text(multicolor_text, { 10 });

		//null::render::background.add_text("rect filled", { 100, 200 }, { }, text_flags);
		//null::render::background.add_rect_filled({ 50, 200 }, { 150, 300 }, color, { 10.f, corner_flags });

		//null::render::background.add_text("rect", { 250, 200 }, { }, text_flags);
		//null::render::background.add_rect({ 200 }, { 300 }, color, 2.f, { 10.f, corner_flags });

		//null::render::background.add_text("circle filled", { 400, 200 }, { }, text_flags);
		//null::render::background.add_circle_filled({ 400, 250 }, color, 50);

		//null::render::background.add_text("circle", { 550, 200 }, { }, text_flags);
		//null::render::background.add_circle({ 550, 250 }, color, 50, 0, 2.f);

		//null::render::background.add_text("multicolor rect filled", { 100, 400 }, { }, text_flags);
		//null::render::background.add_rect_filled_multicolor({ 50, 400 }, { 150, 500 }, { color_t<int>::palette_t::red, color_t<int>::palette_t::green, color_t<int>::palette_t::blue, color_t<int>::palette_t::white }, { 10.f, corner_flags });

		//null::render::background.add_text("multicolor rect", { 250, 400 }, { }, text_flags);
		//null::render::background.add_rect_multicolor({ 200, 400 }, { 300, 500 }, { color_t<int>::palette_t::red, color_t<int>::palette_t::green, color_t<int>::palette_t::blue, color_t<int>::palette_t::white }, 2.f, { 10.f, 20.f, 30.f, 40.f });

		//null::render::background.add_poly_line({ { 400 }, { 550, 400 }, { 550, 500 } }, { }, true, 20.f);

		////@note:	you can use L"" to display cyrillic
		////			or you can add /utf - 8 to C/C++->command line
		//null::render::background.add_text("абвгдеёжзийклмнопрстуфхцчшщъыьэюя", { 500 }, { });

	} null::render::end_frame();
}

int main() {
	window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);

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