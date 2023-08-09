#pragma once
#pragma comment (lib,"d3d9.lib")
#include <d3d9.h>

#include <backend/renderer/renderer.h>

namespace null::render::directx9 {
	struct shared_t {
	public:
		IDirect3DDevice9* device{ };
	} inline shared{ };

	class c_renderer : public backend::i_renderer {
	public:
		IDirect3DStateBlock9* state_block{ };

	public:
		matrix4x4_t get_projection_matrix() const override;

		void set_texture(void* texture) override;
		void set_clip(const rect_t<float>& rect) override;
		void draw_geometry(size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) override;

		void* create_texture(const vec2_t<float>& size, void* data) override;
		void destroy_texture(void* texture) override;

		void setup_state() override;

	private:
		void save_state() override;
		void restore_state() override;

	public:
		bool empty() const override { return !shared.device; }
	};
}