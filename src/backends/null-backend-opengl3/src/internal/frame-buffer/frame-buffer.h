#pragma once
#include <backend/internal/frame-buffer.h>

namespace null::render::backend::opengl3 {
	class c_frame_buffer : public i_frame_buffer {
	public: using i_frame_buffer::i_frame_buffer;
		std::uint32_t fbo{ }, fbo_texture{ }, rbo{ };

	public:
		void* get_texture() override { return (void*)fbo; }
		void* get_renderable_texture() override { return (void*)fbo_texture; }

	public:
		void create() override;
		void destroy() override;

	public:
		void clear() override;
		void set() override;

	public:
		void copy_from_backbuffer() override;
		void copy_from_another_frame_buffer(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) override;

	public:
		bool empty() const override { return fbo == 0; }
	};
}