#pragma once
#include <null-render.h>
#include <backend/backend.h>

#include <internal/frame-buffer/frame-buffer.h>
#include <internal/mesh/mesh.h>
#include <shaders/passthrough-color/passthrough-color.h>
#include <shaders/passthrough-texture/passthrough-texture.h>
#include <shaders/quad-gradient/quad-gradient.h>
#include <shaders/sdf/sdf.h>

namespace null::render::backend::directx9 {
	class c_factory : public i_factory {
	public:
		c_factory(IDirect3DDevice9* device) { shared.device = device; }

	public:
		std::unique_ptr<i_renderer> instance_renderer() override { return std::make_unique<c_renderer>(); }
		std::unique_ptr<backend::c_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
		std::unique_ptr<i_frame_buffer> instance_frame_buffer(const e_frame_buffer_type& type, const e_frame_buffer_flags& flags) override { return std::make_unique<c_frame_buffer>(type, flags); }

		std::unique_ptr<backend::shaders::i_passthrough_color> instance_passthrough_color_shader() override { return std::make_unique<shaders::c_passthrough_color>(); }
		std::unique_ptr<backend::shaders::i_passthrough_texture> instance_passthrough_texture_shader() override { return std::make_unique<shaders::c_passthrough_texture>(); }
		std::unique_ptr<backend::shaders::i_quad_gradient> instance_quad_gradient_shader() override { return std::make_unique<shaders::c_quad_gradient>(); }
		std::unique_ptr<backend::shaders::i_sdf> instance_sdf_shader() override { return std::make_unique<shaders::c_sdf>(); }
	};

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
				utils::logger.log(utils::e_log_type::error, "Direct3DCreate9 return nullptr.");
			if(auto result{ direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &present_parameters, &device) }; FAILED(result))
				utils::logger.log(utils::e_log_type::error, "CreateDevice failed, return code {}.", result);

			factory = std::make_unique<c_factory>(device);
			render::initialize();
			utils::win::c_window::on_create();
		}

		void on_destroy() override {
			utils::win::c_window::on_destroy();

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
						shared::viewport = vec2_t{ LOWORD(l_param), HIWORD(l_param) };
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
			if(auto result{ device->Reset(&present_parameters) }; FAILED(result))
				utils::logger.log(utils::e_log_type::error, "device reset failed, return code {}.", result);
			renderer->create_objects();
		}
	};
}