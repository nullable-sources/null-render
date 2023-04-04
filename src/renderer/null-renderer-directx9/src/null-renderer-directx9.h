#pragma once
#pragma comment (lib,"d3d9.lib")
#include <d3d9.h>

#include <null-render.h>

namespace null::render {
	class c_directx9 : public i_renderer {
	public:
		struct vertex_t {
			vec2_t<float> pos{ }, uv{ };
			sdk::i_color<byte> color{ };
		};

	public:
		IDirect3DDevice9* device{ };
		IDirect3DVertexDeclaration9* vertex_declaration{ };
		IDirect3DVertexBuffer9* vertex_buffer{ };
		IDirect3DIndexBuffer9* index_buffer{ };

	public:
		c_directx9(IDirect3DDevice9* _device = nullptr) : device{ _device } { initialize(); }
		~c_directx9() { shutdown(); }

	public:
		void set_texture(void* texture) override;
		void set_clip(const rect_t<float>& rect) override;
		void draw_geometry(const size_t& vertex_count, const size_t& index_count, const size_t& vertex_offset, const size_t& index_offset) override;

	public:
		void initialize() override;
		void shutdown() override { destroy_objects(); if(device) { device->Release(); device = nullptr; } }

		void begin_frame() override { create_objects(); }
		void end_frame() override { }

		void begin_render() override;
		void end_render() override;

		void setup_state() override;

		void create_objects() override;
		void destroy_objects() override;

		void* create_texture(const vec2_t<float>& size, void* data) override;
		void destroy_texture(void* texture) override;
	} inline* directx9{ };

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

			renderer = std::make_unique<c_directx9>(device);
			renderer->initialize();

			directx9 = (c_directx9*)renderer.get();
			utils::win::c_window::on_create();
		}

		void on_destroy() override {
			utils::win::c_window::on_destroy();
			renderer->shutdown();
			if(direct3d) { direct3d->Release(); direct3d = nullptr; }
		}

		void on_main_loop() override {
			utils::win::c_window::on_main_loop();

			//compile_default_geometry_data();

			device->SetRenderState(D3DRS_ZENABLE, FALSE);
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(clear_color.r, clear_color.g, clear_color.b, clear_color.a), 1.0f, 0);
			if(device->BeginScene() >= 0) {
				renderer->begin_render();
				renderer->end_render();
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