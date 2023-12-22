#pragma once
#include "../internal/mesh.h"
#include "../internal/frame-buffer.h"
#include "../../graphic/commands/geometry/geometry.h"

namespace null::render::backend {
	class c_post_processing : private i_renderer_event_listener {
	public:
		std::array<std::unique_ptr<i_frame_buffer>, 4> frame_buffers{ };
		std::unique_ptr<i_frame_buffer> transfer_buffer{ };

	private:
		std::unique_ptr<c_mesh> mesh{ };
		std::shared_ptr<c_geometry_command> geometry_command{ };

		bool geometry_dirty{ };

	public:
		c_post_processing() {
			renderer_event_dispatcher.attach_listener(e_renderer_event_type::create, this);
			renderer_event_dispatcher.attach_listener(e_renderer_event_type::viewport_resize_end, this);
		}

		virtual ~c_post_processing() {
			renderer_event_dispatcher.detach_listener(e_renderer_event_type::create, this);
			renderer_event_dispatcher.detach_listener(e_renderer_event_type::viewport_resize_end, this);
		}

	public:
		void initialize();

		//@note: the best solution would be to implement the correct blit using the backend and the framebuffers themselves,
		//		 but since it needs to be unified under 3 very different backends, it will be easier to do just that.
		void blit_buffer_region(const std::unique_ptr<i_frame_buffer>& buffer, const rect_t<float>& geometry_region, const rect_t<float>& uvs_region);
		void blit_buffer(const std::unique_ptr<i_frame_buffer>& buffer);

		std::unique_ptr<i_frame_buffer>& at(size_t i) { return frame_buffers[i]; }

	private:
		void* prepare_buffer_texture(const std::unique_ptr<i_frame_buffer>& buffer);

		void generate_geometry(const rect_t<float>& geometry_region, const rect_t<float>& uvs_region);
		void draw_geometry();

		void generate_viewport_geometry();

		void on_create() override;
		void on_viewport_resize_end() override;
	}; inline std::unique_ptr<c_post_processing> post_processing{ };
}