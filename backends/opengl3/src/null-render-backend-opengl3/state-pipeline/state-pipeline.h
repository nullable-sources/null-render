#pragma once
#include <null-render/backend/state-pipeline/state-pipeline.h>

namespace null::render::opengl3 {
	class c_state_pipeline : public backend::i_state_pipeline {
	public:
		struct {
			struct stencil_t {
			public:
				int func{ }, ref{ }, value_mask{ }, writemask{ }, fail{ }, pass_depth_fail{ }, pass_depth_pass{ };
			} stencil_front{ }, stencil_back{ };

			std::uint32_t draw_fbo{ }, read_fbo{ }, rbo{ };
			std::uint32_t active_texture{ }, program{ }, texture{ }, sampler{ }, array_buffer{ }, element_array_buffer{ }, vertex_array_object{ };
			std::uint32_t blend_src_rgb{ }, blend_dst_rgb{ }, blend_src_alpha{ }, blend_dst_alpha{ }, blend_equation_rgb{ }, blend_equation_alpha{ };
			bool enable_blend{ }, enable_cull_face{ }, enable_depth_test{ }, enable_stencil_test{ }, enable_scissor_test{ }, enable_primitive_restart{ };
			int stencil_clear_value{ };
			int viewport[4]{ }, scissor_box[4]{ };
		} saved_state{ };

	public:
		void setup_state() override;
		void save_state() override;
		void restore_state() override;

	public:
		void restore_framebuffer() override;
		void restore_shader() override;
		void restore_mesh() override;
		void restore_texture() override;
	};
}
