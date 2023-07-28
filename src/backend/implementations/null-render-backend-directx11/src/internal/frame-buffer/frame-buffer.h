#pragma once
#include <renderer/renderer.h>
#include <backend/internal/frame-buffer.h>

namespace null::render::backend::directx11 {
	class c_frame_buffer : public i_frame_buffer {
	public: using i_frame_buffer::i_frame_buffer;
		ID3D11RasterizerState* rasterizer_state{ };
		ID3D11Texture2D* render_target_texture{ }, *depth_stencil_view_texture{ };
		ID3D11ShaderResourceView* shader_resource_view{ };
		ID3D11RenderTargetView* render_target{ };
		ID3D11DepthStencilView* depth_stencil_view{ };

	public:
		void* get_surface() override { return render_target_texture; }
		void* get_texture() override { return shader_resource_view; }

	public:
		void on_create() override;
		void on_destroy() override;

	public:
		void clear() override;
		void set() override;

	public:
		void copy_from(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) override;

	public:
		bool empty() const override { return !render_target || !depth_stencil_view; }
	};
}