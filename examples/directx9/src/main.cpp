#define NOMINMAX
#include <Windows.h>
#pragma comment (lib,"d3d9.lib")
#include <d3d9.h>
import null.render;
import null.sdk;

null::render::directx9::c_window window{ };

void main_loop() {
	null::render::begin_frame(window);
	{
		null::render::background_draw_list.prim_reserve(6, 4);
		null::render::background_draw_list.prim_rect({ 10 }, { 30 }, { 0, 255, 255, 150 });
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