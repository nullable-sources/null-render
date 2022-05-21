#define NOMINMAX
#include <Windows.h>
#pragma comment (lib,"d3d9.lib")
#include <d3d9.h>
import null.render;
import null.sdk;

null::render::directx9::c_window window{ };

void main_loop() {
	static null::render::multicolor_text_t multicolor_text{ {
		{ "null-", { } },
		{ "render", { 100, 100, 255 } },
		{ "\n\n", { } },
		{ "null\n", { } },
		{ "render", { 100, 100, 255 } }
	} };

	null::render::begin_frame(window);
	{
		null::render::background_draw_list.draw_text(std::format("fps: {:3.0f}", 1.f / window.time_data.delta_time), { window.get_window_size().x, 10 }, { 255, 255, 255 }, null::e_text_flags{ -null::e_text_flags::aligin_right | -null::e_text_flags::aligin_centre_y | -null::e_text_flags::outline });
		null::render::background_draw_list.draw_text(multicolor_text, { 10 });
		null::render::background_draw_list.draw_rect_filled({ 200 }, { 300 }, { 100, 100, 255 }, 10.f, null::e_corner_flags{ null::e_corner_flags::bot_left | null::e_corner_flags::top_right });
	}
	null::render::end_frame();
	null::render::setup_draw_data();
}

int main(HINSTANCE instance) {
	window = null::render::directx9::c_window{ instance };

	try {
		null::render::global_atlas.add_font_from_file_ttf("C:\\Windows\\fonts\\Tahoma.ttf", 13.f);

		window.create();
		null::render::initialize();
		
		window.main_loop(main_loop);
		
		null::render::shutdown();
		window.destroy();
	} catch(std::exception exp) {
		std::cout << exp.what() << std::endl;
	}
}