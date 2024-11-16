#pragma once
#include "null-render-backend-directx9/state-pipeline/state-pipeline.h"
#include "null-render-backend-directx9/state-pipeline/rasterizer-state/rasterizer-state.h"
#include "null-render-backend-directx9/state-pipeline/blend-state/blend-state.h"
#include "null-render-backend-directx9/state-pipeline/stencil-state/stencil-state.h"
#include "null-render-backend-directx9/internal/frame-buffer/frame-buffer.h"
#include "null-render-backend-directx9/internal/mesh/mesh.h"
#include "null-render-backend-directx9/stencil-buffer/stencil-buffer.h"
#include "null-render-backend-directx9/shaders/passthrough/passthrough.h"
#include "null-render-backend-directx9/shaders/color/color.h"
#include "null-render-backend-directx9/shaders/texture/texture.h"
#include "null-render-backend-directx9/shaders/blur/blur.h"
#include "null-render-backend-directx9/shaders/quad-gradient/quad-gradient.h"
#include "null-render-backend-directx9/shaders/linear-gradient/linear-gradient.h"
#include "null-render-backend-directx9/shaders/radial-gradient/radial-gradient.h"
#include "null-render-backend-directx9/shaders/sdf/sdf.h"

namespace ntl::render::directx9 {
    class c_factory : public backend::i_factory {
    public:
        c_factory(IDirect3DDevice9* device) { shared.device = device; }

    public:
        std::unique_ptr<backend::i_renderer> instance_renderer() override { return std::make_unique<c_renderer>(); }
        std::unique_ptr<backend::c_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
        std::unique_ptr<backend::i_frame_buffer> instance_frame_buffer(const vec2_t<int>& size, backend::e_frame_buffer_type type, backend::e_frame_buffer_flags flags) override { return std::make_unique<c_frame_buffer>(size, type, flags); }
        std::unique_ptr<backend::i_stencil_buffer> instance_stencil_buffer() override { return std::make_unique<c_stencil_buffer>(); }
        
        std::unique_ptr<backend::i_state_pipeline> instance_state_pipeline() override { return std::make_unique<c_state_pipeline>(); }
        std::unique_ptr<backend::i_rasterizer_state> instance_rasterizer_state() override { return std::make_unique<c_rasterizer_state>(); }
        std::unique_ptr<backend::i_blend_state> instance_blend_state() override { return std::make_unique<c_blend_state>(); }
        std::unique_ptr<backend::i_stencil_state> instance_stencil_state() override { return std::make_unique<c_stencil_state>(); }

        std::unique_ptr<backend::i_passthrough_shader> instance_passthrough_shader() override { return std::make_unique<c_passthrough_shader>(); }
        std::unique_ptr<backend::i_color_shader> instance_color_shader() override { return std::make_unique<c_color_shader>(); }
        std::unique_ptr<backend::i_texture_shader> instance_texture_shader() override { return std::make_unique<c_texture_shader>(); }
        std::unique_ptr<backend::i_blur_shader> instance_blur_shader() override { return std::make_unique<c_blur_shader>(); }
        std::unique_ptr<backend::i_quad_gradient_shader> instance_quad_gradient_shader() override { return std::make_unique<c_quad_gradient_shader>(); }
        std::unique_ptr<backend::i_linear_gradient_shader> instance_linear_gradient_shader() override { return std::make_unique<c_linear_gradient_shader>(); }
        std::unique_ptr<backend::i_radial_gradient_shader> instance_radial_gradient_shader() override { return std::make_unique<c_radial_gradient_shader>(); }
        std::unique_ptr<backend::i_sdf_shader> instance_sdf_shader() override { return std::make_unique<c_sdf_shader>(); }
        std::unique_ptr<backend::i_msdf_shader> instance_msdf_shader() override { return std::make_unique<c_msdf_shader>(); }
        std::unique_ptr<backend::i_mtsdf_shader> instance_mtsdf_shader() override { return std::make_unique<c_mtsdf_shader>(); }
    };

    class c_window : public win::c_window {
    public: using win::c_window::c_window;
    public:
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
                sdk::logger(sdk::e_log_type::error, "Direct3DCreate9 return nullptr.");
            if(auto result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &present_parameters, &device); FAILED(result))
                sdk::logger(sdk::e_log_type::error, "CreateDevice failed, return code {}.", result);

            backend::factory = std::make_unique<c_factory>(device);
            render::initialize(size);
            win::c_window::on_create();
        }

        void on_destroy() override {
            win::c_window::on_destroy();

            if(direct3d) { direct3d->Release(); direct3d = nullptr; }
        }

        void on_main_loop() override {
            device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(clear_color.r, clear_color.g, clear_color.b, clear_color.a), 1.0f, 0);
            if(device->BeginScene() >= 0) {
                //device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(clear_color.r, clear_color.g, clear_color.b, clear_color.a), 1.0f, 0);
                win::c_window::on_main_loop();
                device->EndScene();
            }

            if(device->Present(nullptr, nullptr, nullptr, nullptr) == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
                reset_device();
        }

        std::vector<int> on_wnd_proc(HWND _wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) override {
            static bool resize_start = false, resize_buffers = false, enter_sizemove = false;
            std::vector<int> callback_results = win::c_window::on_wnd_proc(_wnd_handle, msg, w_param, l_param);
            if(!device) return callback_results;

            switch(msg) {
                case WM_ENTERSIZEMOVE: enter_sizemove = true; return { 0 };
                case WM_SIZE: {
                    if(w_param == SIZE_MINIMIZED) return { 0 };
                    backend::renderer->begin_resize_viewport(vec2_t(LOWORD(l_param), HIWORD(l_param)));

                    if(w_param == SIZE_MAXIMIZED || (w_param == SIZE_RESTORED && !enter_sizemove)) resize_buffers = true;
                    else resize_start = true;
                } return { 0 };
                case WM_EXITSIZEMOVE: {
                    enter_sizemove = false;
                    if(resize_start) {
                        resize_buffers = true;
                        resize_start = false;
                    }
                } return { 0 };
            }

            if(resize_buffers) {
                present_parameters.BackBufferWidth = shared::viewport.x;
                present_parameters.BackBufferHeight = shared::viewport.y;
                reset_device();
                backend::renderer->end_resize_viewport();
                resize_buffers = false;
            }

            return callback_results;
        }

        void reset_device() {
            backend::renderer->destroy_objects();
            if(auto result = device->Reset(&present_parameters); FAILED(result))
                sdk::logger(sdk::e_log_type::error, "device reset failed, return code {}.", result);
            backend::renderer->create_objects();
        }
    };
}