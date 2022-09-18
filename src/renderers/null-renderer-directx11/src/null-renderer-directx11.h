#pragma once
#pragma comment (lib, "d3d11.lib")
#include <d3d11.h>

#include <null-render.h>

namespace null::render::directx11 {
	struct vertex_t {
		float pos[3]{ };
		std::uint32_t color{ };
		float uv[2]{ };
	};

	inline ID3D11RenderTargetView* main_render_target_view;
	inline ID3D11Device* device;
	inline ID3D11DeviceContext* context;
	inline IDXGIFactory* factory;
	inline ID3D11Buffer* vertex_buffer;
	inline ID3D11Buffer* index_buffer;
	inline ID3D11InputLayout* input_layout;
	inline ID3D11SamplerState* font_sampler;
	inline ID3D11ShaderResourceView* font_texture_view;
	inline ID3D11RasterizerState* rasterizer_state;
	inline ID3D11BlendState* blend_state;
	inline ID3D11DepthStencilState* depth_stencil_state;
	inline IDXGISwapChain* swap_chain;

	void render_draw_data(c_draw_list::draw_data_t* _draw_data = &draw_data);
	void setup_render_state(c_draw_list::draw_data_t* _draw_data = &draw_data);
	void create_fonts_texture();
	void create_device_objects();
	void invalidate_device_objects();

	void initialize(ID3D11Device* _device = nullptr, ID3D11DeviceContext* _context = nullptr);
	void shutdown();

	static void begin_frame() { if(!font_sampler) create_device_objects(); }

	class c_window : public utils::win::c_window {
	public: using utils::win::c_window::c_window;
		color_t clear_color{ 0.07f, 0.07f, 0.07f };

		DXGI_SWAP_CHAIN_DESC swap_chain_desc{
			.BufferDesc{
				.Width{ 0 },
				.Height{ 0 },

				.RefreshRate{
					.Numerator{ 60 },
					.Denominator{ 1 }
				},

				.Format{ DXGI_FORMAT_R8G8B8A8_UNORM }
			},

			.SampleDesc{
				.Count{ 1 },
				.Quality{ 0 }
			},

			.BufferUsage{ DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT },
			.BufferCount{ 2 },
			.Windowed{ true },
			.SwapEffect{ DXGI_SWAP_EFFECT_DISCARD },
			.Flags{ DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_RESTRICT_SHARED_RESOURCE_DRIVER }
		};

	public:
		void on_create() override {
			swap_chain_desc.OutputWindow = wnd_handle;

			D3D_FEATURE_LEVEL feature_level;
			if(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, std::vector<D3D_FEATURE_LEVEL>{ D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 }.data(), 2,
				D3D11_SDK_VERSION, & swap_chain_desc, & swap_chain, & device, & feature_level, & context) != S_OK)
				throw std::runtime_error{ "cant create device and swap chain" };

			create_render_target();

			initialize();

			utils::win::c_window::on_create();
		}

		void on_destroy() override {
			utils::win::c_window::on_destroy();
			destroy_render_target();
			if(device) { device->Release(); device = nullptr; }
			if(context) { context->Release(); context = nullptr; }
			if(swap_chain) { swap_chain->Release(); swap_chain = nullptr; }
		}

		void on_main_loop() override {
			begin_frame();

			utils::win::c_window::on_main_loop();

			setup_draw_data();

			context->OMSetRenderTargets(1, &main_render_target_view, nullptr);
			context->ClearRenderTargetView(main_render_target_view, clear_color.channels.data());

			render_draw_data();

			swap_chain->Present(1, 0);
		}

		std::vector<std::any> on_wnd_proc(HWND _wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) override {
			std::vector<std::any> callback_results{ utils::win::c_window::on_wnd_proc(_wnd_handle, msg, w_param, l_param) };
			switch(msg) {
				case WM_SIZE: {
					if(device && w_param != SIZE_MINIMIZED) {
						destroy_render_target();

						swap_chain->ResizeBuffers(0, (std::uint32_t)LOWORD(l_param), (std::uint32_t)HIWORD(l_param), DXGI_FORMAT_UNKNOWN, 0);

						create_render_target();
					}
				} return std::vector<std::any>{ 0 };
			}

			return callback_results;
		}

		void destroy_render_target() { if(main_render_target_view) { main_render_target_view->Release(); main_render_target_view = nullptr; } }
		void create_render_target() {
			ID3D11Texture2D* back_buffer{ };
			swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
			device->CreateRenderTargetView(back_buffer, nullptr, &main_render_target_view);
			back_buffer->Release();
		}
	};
}