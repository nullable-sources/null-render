#pragma once
#pragma comment (lib, "d3d11.lib")
#include <d3d11.h>

#include <null-render.h>

namespace null::renderer {
	class c_directx11 : public i_renderer {
	public:
		struct vertex_t {
			float pos[3]{ };
			std::uint32_t color{ };
			float uv[2]{ };
		};

	public:
		ID3D11Device* device{ };
		ID3D11DeviceContext* context{ };
		IDXGIFactory* factory{ };
		ID3D11Buffer* vertex_buffer{ }, * index_buffer{ };
		ID3D11InputLayout* input_layout{ };
		ID3D11SamplerState* font_sampler{ };
		ID3D11ShaderResourceView* font_texture_view{ };
		ID3D11RasterizerState* rasterizer_state{ };
		ID3D11BlendState* blend_state{ };
		ID3D11DepthStencilState* depth_stencil_state{ };
	
	public:
		c_directx11(ID3D11Device* _device = nullptr, ID3D11DeviceContext* _context = nullptr) : device{ _device }, context{ _context } { initialize(); }
		~c_directx11() { shutdown(); }

	public:
		void initialize() override;
		void shutdown() override;

		void begin_frame() override { if(!font_sampler) create_objects(); }
		void end_frame() override { }

		void render(const draw_data_t& _draw_data = draw_data) override;
		void setup_state() override;

		void create_objects() override;
		void destroy_objects() override;

	public:
		void create_fonts_texture();
	}; inline std::unique_ptr<c_directx11> directx11{ };

	class c_window : public utils::win::c_window {
	public: using utils::win::c_window::c_window;
		color_t<float> clear_color{ 0.07f, 0.07f, 0.07f };

		ID3D11Device* device{ };
		ID3D11DeviceContext* context{ };
		IDXGISwapChain* swap_chain{ };
		ID3D11RenderTargetView* render_target{ };
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
				D3D11_SDK_VERSION, & swap_chain_desc, & swap_chain, &device, & feature_level, &context) != S_OK)
				throw std::runtime_error{ "cant create device and swap chain" };

			create_render_target();

			directx11 = std::make_unique<c_directx11>(device, context);
			renderer = directx11.get();

			utils::win::c_window::on_create();
		}

		void on_destroy() override {
			utils::win::c_window::on_destroy();
			destroy_render_target();
			renderer->shutdown();
			if(swap_chain) { swap_chain->Release(); swap_chain = nullptr; }
		}

		void on_main_loop() override {
			utils::win::c_window::on_main_loop();

			setup_default_draw_data();

			context->OMSetRenderTargets(1, &render_target, nullptr);
			context->ClearRenderTargetView(render_target, clear_color.channels.data());

			renderer->render();

			swap_chain->Present(1, 0);
		}

		std::vector<int> on_wnd_proc(HWND _wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) override {
			std::vector<int> callback_results{ utils::win::c_window::on_wnd_proc(_wnd_handle, msg, w_param, l_param) };
			switch(msg) {
				case WM_SIZE: {
					if(device && w_param != SIZE_MINIMIZED) {
						destroy_render_target();

						swap_chain->ResizeBuffers(0, (std::uint32_t)LOWORD(l_param), (std::uint32_t)HIWORD(l_param), DXGI_FORMAT_UNKNOWN, 0);

						create_render_target();
					}
				} return { 0 };
			}

			return callback_results;
		}

		void destroy_render_target() { if(render_target) { render_target->Release(); render_target = nullptr; } }
		void create_render_target() {
			ID3D11Texture2D* back_buffer{ };
			swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
			device->CreateRenderTargetView(back_buffer, nullptr, &render_target);
			back_buffer->Release();
		}
	};
}