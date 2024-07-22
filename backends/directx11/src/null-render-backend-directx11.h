#pragma once


#include "null-render-backend-directx11/internal/frame-buffer/frame-buffer.h"
#include "null-render-backend-directx11/internal/stencil-buffer/stencil-buffer.h"
#include "null-render-backend-directx11/internal/mesh/mesh.h"
#include "null-render-backend-directx11/state-pipeline/state-pipeline.h"
#include "null-render-backend-directx11/state-pipeline/rasterizer-state/rasterizer-state.h"
#include "null-render-backend-directx11/shaders/passthrough/passthrough.h"
#include "null-render-backend-directx11/shaders/color/color.h"
#include "null-render-backend-directx11/shaders/texture/texture.h"
#include "null-render-backend-directx11/shaders/blur/blur.h"
#include "null-render-backend-directx11/shaders/quad-gradient/quad-gradient.h"
#include "null-render-backend-directx11/shaders/linear-gradient/linear-gradient.h"
#include "null-render-backend-directx11/shaders/radial-gradient/radial-gradient.h"
#include "null-render-backend-directx11/shaders/sdf/sdf.h"

namespace null::render::directx11 {
    class c_factory : public backend::i_factory {
    public:
        c_factory(dx_device_t* device, dx_device_context_t* context, IDXGISwapChain* swap_chain) {
            shared.device = device; shared.context = context, shared.swap_chain = swap_chain;
        }

    public:
        std::unique_ptr<backend::i_renderer> instance_renderer() override { return std::make_unique<c_renderer>(); }
        std::unique_ptr<backend::c_mesh> instance_mesh() override { return std::make_unique<c_mesh>(); }
        std::unique_ptr<backend::i_frame_buffer> instance_frame_buffer(const vec2_t<int>& size, backend::e_frame_buffer_type type, backend::e_frame_buffer_flags flags) override { return std::make_unique<c_frame_buffer>(size, type, flags); }
        std::unique_ptr<backend::i_stencil_buffer> instance_stencil_buffer() override { return std::make_unique<c_stencil_buffer>(); }
        
        std::unique_ptr<backend::i_state_pipeline> instance_state_pipeline() override { return std::make_unique<c_state_pipeline>(); }
        std::unique_ptr<backend::i_rasterizer_state> instance_rasterizer_state() override { return std::make_unique<c_rasterizer_state>(); }

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

    class c_window : public utils::win::c_window {
    public: using utils::win::c_window::c_window;
    public:
        color_t<float> clear_color{ 0.07f, 0.07f, 0.07f };

        dx_device_t* device{ };
        dx_device_context_t* context{ };


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

            static const std::vector<D3D_FEATURE_LEVEL> creating_feature_level{
#ifdef NULL_RENDER_DX11_ALLOW_FEATURE_11_1
                D3D_FEATURE_LEVEL_11_1,
#endif
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_0
            };

            ID3D11Device* creation_device{ };
            ID3D11DeviceContext* creation_context{ };

            D3D_FEATURE_LEVEL feature_level;
            if(auto result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, creating_feature_level.data(), 2, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &creation_device, &feature_level, &creation_context); FAILED(result)) {
                utils::logger(utils::e_log_type::error, "D3D11CreateDeviceAndSwapChain failed, return code {}.", result);
            }

#ifdef NULL_RENDER_DX11_ALLOW_FEATURE_11_1
            creation_device->QueryInterface(__uuidof (dx_device_t), (void**)&device);
            creation_context->QueryInterface(__uuidof (dx_device_context_t), (void**)&context);
            creation_device->Release();
            creation_context->Release();
#else
            device = creation_device;
            context = creation_context;
#endif

            backend::factory = std::make_unique<c_factory>(device, context, swap_chain);
            render::initialize(size);

            utils::win::c_window::on_create();
        }

        void on_destroy() override {
            utils::win::c_window::on_destroy();

            if(swap_chain) { swap_chain->Release(); swap_chain = nullptr; }
        }

        void on_main_loop() override {
            backend::rendering_buffer->clear();
            utils::win::c_window::on_main_loop();

            if(auto result = swap_chain->Present(1, 0); FAILED(result))
                utils::logger(utils::e_log_type::error, "Present failed, return code {}.", result);
        }

        std::vector<int> on_wnd_proc(HWND _wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) override {
            std::vector<int> callback_results = utils::win::c_window::on_wnd_proc(_wnd_handle, msg, w_param, l_param);
            switch(msg) {
                case WM_SIZE: {
                    if(device && w_param != SIZE_MINIMIZED) {
                        backend::renderer->begin_resize_viewport(vec2_t<std::uint32_t>(LOWORD(l_param), HIWORD(l_param)));
                        swap_chain->ResizeBuffers(0, render::shared::viewport.x, render::shared::viewport.y, DXGI_FORMAT_UNKNOWN, 0);
                        backend::renderer->end_resize_viewport();
                    }
                } return { 0 };
            }

            return callback_results;
        }
    };
}