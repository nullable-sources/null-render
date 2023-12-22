#pragma once
#include <null-render.h>

namespace null::render::opengl3 {
	class c_frame_buffer : public backend::i_frame_buffer {
	public: using i_frame_buffer::i_frame_buffer;
		std::uint32_t fbo{ }, fbo_attachment{ }, depth_buffer{ };

	public:
		void* get_surface() override { return (void*)fbo; }
		void* get_texture() override { return (void*)fbo_attachment; }

	public:
		void create() override;
		void destroy() override;

		void clear() override;
		void use() override;

		void copy_from(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) override;

	public:
		bool empty() const override { return fbo == 0; }
	};
}