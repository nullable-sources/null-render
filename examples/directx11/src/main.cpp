﻿#include <iostream>
#include <null-renderer-directx11.h>

null::render::c_window window{ };
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
		brush.set_color({ 100, 255, 255 });
		null::render::background.add_quad({ { 200, 200 }, { 300, 150} }, { { 150, 400 }, { 300, 450 } }, brush);

		null::render::quad_gradient_brush_t gradient_brush{ };
		gradient_brush
			.set_top_left_color({ 255, 255, 255 })
			.set_top_right_color({ 255, 100, 0 })
			.set_bottom_left_color({ 255, 0, 100 })
			.set_bottom_right_color({ 100, 100, 255 });
		null::render::background.add_rect({ 500, 200 }, { 600, 300 }, gradient_brush);

		null::render::sdf_text_style_t text_style{ };
		text_style
			.set_color({ 255, 100, 100 })
			.set_size(40.f)
			.set_outline(1.f, { 255, 200, 200 }, { 100, 100, 255 });
		null::render::background.add_text("sdf text", { }, text_style);
	} null::render::end_frame();
}

int main(HINSTANCE instance) {
	window = null::render::c_window{ instance };

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

		window.create();

		window.main_loop();
		window.destroy();
	} catch(const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
}