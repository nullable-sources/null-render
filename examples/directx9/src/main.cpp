#include <iostream>
#include <null-renderer-directx9.h>

null::render::directx9::c_window window{ };
null::render::c_draw_list custom_layer{ };

void main_loop() {
	static const null::e_text_flags text_flags{ null::e_text_flags::aligin_bottom | null::e_text_flags::aligin_center_x };
	static const null::e_corner_flags corner_flags{ null::e_corner_flags::bot_left | null::e_corner_flags::top_right };

	static const color_t color{ 100, 100, 255 };

	static const null::render::multicolor_text_t<std::string> multicolor_text{ {
		{ "multicolored \n\n", { } },
		{ "text", color }
	} };

	null::render::begin_frame(window);
	{
		custom_layer.draw_text("text drawed by custom draw_list", { 100, 10 }, { });
		null::render::background_layer.draw_text(std::format("[ directx9 ] fps: {:3.0f}", 1.f / window.time_data.delta_time), { window.get_window_size().x, 10 }, { }, null::e_text_flags{ -null::e_text_flags::aligin_right | -null::e_text_flags::aligin_center_y | -null::e_text_flags::outline });
		null::render::background_layer.draw_text(multicolor_text, { 10 });

		null::render::background_layer.draw_poly_line({ { 100 }, { 300 }, {300, 200} }, { }, true);

		null::render::background_layer.draw_text("rect filled", { 100, 200 }, { }, text_flags);
		null::render::background_layer.draw_rect_filled({ 50, 200 }, { 150, 300 }, color, 10.f, corner_flags);

		null::render::background_layer.draw_text("rect", { 250, 200 }, { }, text_flags);
		null::render::background_layer.draw_rect({ 200 }, { 300 }, color, 2.f, 10.f, corner_flags);

		null::render::background_layer.draw_text("circle filled", { 400, 200 }, { }, text_flags);
		null::render::background_layer.draw_circle_filled({ 400, 250 }, color, 50);

		null::render::background_layer.draw_text("circle", { 550, 200 }, { }, text_flags);
		null::render::background_layer.draw_circle({ 550, 250 }, color, 50, 0, 2.f);

		//@note:	you can use L"" to display cyrillic
		//			or you can add /utf - 8 to C/C++->command line
		null::render::background_layer.draw_text("абвгдеёжзийклмнопрстуфхцчшщъыьэюя", { 500 }, { });
	}
	null::render::end_frame();
}

int main(HINSTANCE instance) {
	window = null::render::directx9::c_window{ instance };

	window.callbacks.add<void()>(utils::e_window_callbacks::on_main_loop, main_loop);

	try {
		null::render::global_atlas.add_font_from_file_ttf("C:\\Windows\\fonts\\Tahoma.ttf", 13.f, nullptr, null::render::c_font::glyph_t::ranges_cyrillic());

		window.create();
		null::render::custom_layers.push_back(&custom_layer);

		window.main_loop();

		null::render::global_atlas.clear_input_data();

		window.destroy();
	} catch(const std::exception& exp) {
		std::cout << exp.what() << std::endl;
	}
}