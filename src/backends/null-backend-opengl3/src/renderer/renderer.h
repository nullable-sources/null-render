#pragma once
#include <backend/renderer/renderer.h>

namespace null::render::backend::opengl3 {
	class c_renderer : public i_renderer {
	public:
		struct {
			std::uint32_t active_texture{ }, program{ }, texture{ }, sampler{ }, array_buffer{ }, vertex_array_object{ };
			std::uint32_t blend_src_rgb{ }, blend_dst_rgb{ }, blend_src_alpha{ }, blend_dst_alpha{ }, blend_equation_rgb{ }, blend_equation_alpha{ };
			bool enable_blend{ }, enable_cull_face{ }, enable_depth_test{ }, enable_stencil_test{ }, enable_scissor_test{ }, enable_primitive_restart{ };
			int viewport[4]{ }, scissor_box[4]{ };
		} saved_state{ };

	public:
		void set_texture(void* texture) override;
		void set_clip(const rect_t<float>& rect) override;
		void draw_geometry(const size_t& vertex_count, const size_t& index_count, const size_t& vertex_offset, const size_t& index_offset) override;

		void* create_texture(const vec2_t<float>& size, void* data) override;
		void destroy_texture(void* texture) override;

	private:
		void save_state() override;
		void restore_state() override;
		void setup_state() override;

	public:
		bool empty() const override { return false; }
	} inline* opengl3{ };
}