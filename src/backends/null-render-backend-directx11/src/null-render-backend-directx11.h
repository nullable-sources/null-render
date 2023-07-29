#pragma once
#include <null-render.h>
#include <backend/backend.h>

#include <internal/frame-buffer/frame-buffer.h>
#include <internal/mesh/mesh.h>
#include <shaders/passthrough-color/passthrough-color.h>
#include <shaders/passthrough-texture/passthrough-texture.h>
#include <shaders/quad-gradient/quad-gradient.h>
#include <shaders/linear-gradient/linear-gradient.h>
#include <shaders/sdf/sdf.h>

namespace null::render::backend::directx11 {
	class c_factory : public i_factory {
	public:
		c_factory(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swap_chain) {
			shared.device = device; shared.context = context, shared.swap_chain = swap_chain;
		}

	public:
		std::unique_ptr<i_renderer> instance_renderer() override { return std::make_unique<c_renderer>(); }
		std::unique_ptr<backend::c_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
		std::unique_ptr<i_frame_buffer> instance_frame_buffer(const vec2_t<int>& size, e_frame_buffer_type type, e_frame_buffer_flags flags) override { return std::make_unique<c_frame_buffer>(size, type, flags); }

		std::unique_ptr<backend::shaders::i_passthrough_color> instance_passthrough_color_shader() override { return std::make_unique<shaders::c_passthrough_color>(); }
		std::unique_ptr<backend::shaders::i_passthrough_texture> instance_passthrough_texture_shader() override { return std::make_unique<shaders::c_passthrough_texture>(); }
		std::unique_ptr<backend::shaders::i_quad_gradient> instance_quad_gradient_shader() override { return std::make_unique<shaders::c_quad_gradient>(); }
		std::unique_ptr<backend::shaders::i_linear_gradient> instance_linear_gradient_shader() override { return std::make_unique<shaders::c_linear_gradient>(); }
		std::unique_ptr<backend::shaders::i_sdf> instance_sdf_shader() override { return std::make_unique<shaders::c_sdf>(); }
	};

	class c_window : public utils::win::c_window {
	public: using utils::win::c_window::c_window;
		color_t<float> clear_color{ 0.07f, 0.07f, 0.07f };

		ID3D11Device* device{ };
		ID3D11DeviceContext* context{ };
		IDXGISwapChain* swap_chain{ };
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
			if(auto result{ D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, std::vector<D3D_FEATURE_LEVEL>{ D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 }.data(), 2,
				D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &device, &feature_level, &context) }; FAILED(result)) {
				utils::logger(utils::e_log_type::error, "D3D11CreateDeviceAndSwapChain failed, return code {}.", result);
			}

			factory = std::make_unique<c_factory>(device, context, swap_chain);
			render::initialize(*this);

			utils::win::c_window::on_create();
		}

		void on_destroy() override {
			utils::win::c_window::on_destroy();
			
			if(swap_chain) { swap_chain->Release(); swap_chain = nullptr; }
		}

		void on_main_loop() override {
			backend::rendering_buffer->clear();
			utils::win::c_window::on_main_loop();

			if(auto result{ swap_chain->Present(1, 0) }; FAILED(result))
				utils::logger(utils::e_log_type::error, "Present failed, return code {}.", result);
		}

		std::vector<int> on_wnd_proc(HWND _wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) override {
			std::vector<int> callback_results{ utils::win::c_window::on_wnd_proc(_wnd_handle, msg, w_param, l_param) };
			switch(msg) {
				case WM_SIZE: {
					if(device && w_param != SIZE_MINIMIZED) {
						msaa_buffer->on_destroy();
						rendering_buffer->on_destroy();

						render::shared::viewport = vec2_t{ (std::uint32_t)LOWORD(l_param), (std::uint32_t)HIWORD(l_param) };
						swap_chain->ResizeBuffers(0, (std::uint32_t)render::shared::viewport.x, (std::uint32_t)render::shared::viewport.y, DXGI_FORMAT_UNKNOWN, 0);

						msaa_buffer->on_create();
						rendering_buffer->on_create();
					}
				} return { 0 };
			}

			return callback_results;
		}
	};
}