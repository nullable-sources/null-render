#pragma once
#include "../../renderer/renderer.h"

namespace null::render::directx9 {
	class c_frame_buffer : public backend::i_frame_buffer {
	public: using i_frame_buffer::i_frame_buffer;
		IDirect3DTexture9* texture{ };
		IDirect3DSurface9* surface{ };

	public:
		void* get_surface() override { return surface; }
		void* get_texture() override { return texture; }

	public:
		void create() override;
		void destroy() override;

		void clear() override;
		void use() override;

		void copy_from(i_frame_buffer* another_frame_buffer) override;
		void blit_region_from(i_frame_buffer* another_frame_buffer, const vec2_t<int>& blit_offset, const rect_t<int>& region) override;
		void copy_in_texture(void* texture, const rect_t<int>& region) override;

	public:
		bool empty() const override { return !surface; }
	};
}