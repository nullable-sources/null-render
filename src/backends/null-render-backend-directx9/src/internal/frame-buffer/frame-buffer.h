#pragma once
#include <renderer/renderer.h>
#include <backend/internal/frame-buffer.h>

namespace null::render::backend::directx9 {
	class c_frame_buffer : public i_frame_buffer {
	public: using i_frame_buffer::i_frame_buffer;
		IDirect3DTexture9* texture{ };
		IDirect3DSurface9* surface{ };

	public:
		void* get_surface() override { return surface; }
		void* get_texture() override { return texture; }

	public:
		void create() override;
		void destroy() override;

	public:
		void clear() override;
		void set() override;

	public:
		void copy_from(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) override;

	public:
		bool empty() const override { return !surface; }
	};
}