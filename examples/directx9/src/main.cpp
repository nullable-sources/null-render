#include <iostream>
#include <null-renderer-directx9.h>

null::renderer::c_window window{ };
null::render::c_geometry_buffer custom_buffer{ };
utils::c_cumulative_time_measurement frame_counter{ 60 };

void main_loop() {
	static const null::render::e_text_flags text_flags{ null::render::e_text_flags::aligin_bottom | null::render::e_text_flags::aligin_center_x };
	static const null::render::e_corner_flags corner_flags{ null::render::e_corner_flags::bot_left | null::render::e_corner_flags::top_right };

	static const color_t color{ 100, 100, 255 };

	static const null::render::multicolor_text_t<std::string> multicolor_text{ {
		{ "multicolored \n\n", { } },
		{ "text", color }
	} };

	null::render::begin_frame(window); {
		custom_buffer.add_text("text drawed by custom geometry buffer", { 100, 10 }, { });
		null::render::background.add_text(std::format("[ directx9 ] fps: {:3.0f}", 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count()), { (float)window.get_window_size().x, 10.f }, { }, null::render::e_text_flags{ -null::render::e_text_flags::aligin_right | -null::render::e_text_flags::aligin_center_y | -null::render::e_text_flags::outline });
		null::render::background.add_text(multicolor_text, { 10 });

		null::render::background.add_text("rect filled", { 100, 200 }, { }, text_flags);
		null::render::background.add_rect_filled({ 50, 200 }, { 150, 300 }, color, 10.f, corner_flags);

		null::render::background.add_text("rect", { 250, 200 }, { }, text_flags);
		null::render::background.add_rect({ 200 }, { 300 }, color, 2.f, 10.f, corner_flags);

		null::render::background.add_text("circle filled", { 400, 200 }, { }, text_flags);
		null::render::background.add_circle_filled({ 400, 250 }, color, 50);

		null::render::background.add_text("circle", { 550, 200 }, { }, text_flags);
		null::render::background.add_circle({ 550, 250 }, color, 50, 0, 2.f);

		null::render::background.add_text("multicolor rect filled", { 100, 400 }, { }, text_flags);
		null::render::background.add_rect_filled_multicolor({ 50, 400 }, { 150, 500 }, { color_t<int>::palette_t::red, color_t<int>::palette_t::green, color_t<int>::palette_t::blue, color_t<int>::palette_t::white }, 10.f, corner_flags);

		null::render::background.add_text("multicolor rect", { 250, 400 }, { }, text_flags);
		null::render::background.add_rect_multicolor({ 200, 400 }, { 300, 500 }, { color_t<int>::palette_t::red, color_t<int>::palette_t::green, color_t<int>::palette_t::blue, color_t<int>::palette_t::white }, 2.f, 10.f, corner_flags);

		null::render::background.add_poly_line({ { 400 }, { 550, 400 }, { 550, 500 } }, { }, true);

		//@note:	you can use L"" to display cyrillic
		//			or you can add /utf - 8 to C/C++->command line
		null::render::background.add_text("абвгдеёжзийклмнопрстуфхцчшщъыьэюя", { 500 }, { });
	} null::render::end_frame();
}

int main(HINSTANCE instance) {
	window = null::renderer::c_window{ instance };

	window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);

	try {
		null::render::c_font::config_t config{ .glyph_config{ .ranges{ null::render::c_font::glyph_t::ranges_cyrillic() } } };
		null::render::atlas.add_font_default(&config);

		window.create();
		null::render::custom_buffers.push_back(&custom_buffer);

		window.main_loop();
		window.destroy();
	} catch(const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
}