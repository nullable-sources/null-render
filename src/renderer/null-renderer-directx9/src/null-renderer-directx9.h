#pragma once
#pragma comment (lib,"d3d9.lib")
#include <d3d9.h>

#include <null-render.h>

namespace null::renderer {
	class c_directx9 : public i_renderer {
	public:
		struct vertex_t {
			float pos[3]{ };
			std::uint32_t color{ };
			float uv[2]{ };
		};

	public:
		IDirect3DDevice9* device{ };
		IDirect3DTexture9* font_texture{ };
		IDirect3DVertexDeclaration9* vtx_declaration{ };
		IDirect3DVertexBuffer9* vtx_buffer{ };
		IDirect3DIndexBuffer9* idx_buffer{ };

	public:
		c_directx9(IDirect3DDevice9* _device = nullptr) : device{ _device } { initialize(); }
		~c_directx9() { shutdown(); }

	public:
		void initialize() override { if(device) device->AddRef(); }
		void shutdown() override { destroy_objects(); if(device) { device->Release(); device = nullptr; } }

		void begin_frame() override { if(!font_texture) create_objects(); }
		void end_frame() override { }

		void render(const compiled_geometry_data_t& _compiled_geometry_data = compiled_geometry_data) override;
		void setup_state() override;

		void create_objects() override;
		void destroy_objects() override;

	public:
		void create_fonts_texture();
	}; inline std::unique_ptr<c_directx9> directx9{ };

	class c_window : public utils::win::c_window {
	public: using utils::win::c_window::c_window;
		color_t<int> clear_color{ 18, 18, 18 };
		IDirect3DDevice9* device{ };
		IDirect3D9* direct3d{ };
		D3DPRESENT_PARAMETERS present_parameters{
			.BackBufferFormat{ D3DFMT_UNKNOWN },
			.SwapEffect{ D3DSWAPEFFECT_DISCARD },
			.Windowed{ true },
			.EnableAutoDepthStencil{ true },
			.AutoDepthStencilFormat{ D3DFMT_D16 },
			.PresentationInterval{ D3DPRESENT_INTERVAL_ONE }
		};

	public:
		void on_create() override {
			if(!(direct3d = Direct3DCreate9(D3D_SDK_VERSION)))
				throw std::runtime_error{ "cannot create direct3d" };
			if(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &present_parameters, &device) < 0)
				throw std::runtime_error{ "cannot create device" };
			directx9 = std::make_unique<c_directx9>(device);
			renderer = directx9.get();
			utils::win::c_window::on_create();
		}

		void on_destroy() override {
			utils::win::c_window::on_destroy();
			renderer->shutdown();
			if(direct3d) { direct3d->Release(); direct3d = nullptr; }
		}

		void on_main_loop() override {
			utils::win::c_window::on_main_loop();

			compile_default_geometry_data();

			device->SetRenderState(D3DRS_ZENABLE, FALSE);
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(clear_color.r, clear_color.g, clear_color.b, clear_color.a), 1.0f, 0);
			if(device->BeginScene() >= 0) {
				renderer->render();
				device->EndScene();
			}

			if(device->Present(nullptr, nullptr, nullptr, nullptr) == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				reset_device();
		}

		std::vector<int> on_wnd_proc(HWND _wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) override {
			std::vector<int> callback_results{ utils::win::c_window::on_wnd_proc(_wnd_handle, msg, w_param, l_param) };
			switch(msg) {
				case WM_SIZE: {
					if(device && w_param != SIZE_MINIMIZED) {
						present_parameters.BackBufferWidth = LOWORD(l_param);
						present_parameters.BackBufferHeight = HIWORD(l_param);
						reset_device();
					}
				} return { 0 };
			}

			return callback_results;
		}

		void reset_device() {
			renderer->destroy_objects();
			if(device->Reset(&present_parameters) == D3DERR_INVALIDCALL) throw std::runtime_error{ "device->Reset == D3DERR_INVALIDCALL" };
			renderer->create_objects();
		}
	};
}