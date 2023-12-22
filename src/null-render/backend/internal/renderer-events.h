#pragma once
#include <null-sdk.h>

namespace null::render::backend {
	enum class e_renderer_event_type { create, destroy, begin_render, end_render, setup_state, viewport_resize_begin, viewport_resize_end };
	class c_renderer_event_dispatcher : public utils::c_event_dispatcher<e_renderer_event_type> {
	public:
		void create() { dispatch_event(e_renderer_event_type::create, { }); }
		void destroy() { dispatch_event(e_renderer_event_type::destroy, { }); }
		void begin_render() { dispatch_event(e_renderer_event_type::begin_render, { }); }
		void end_render() { dispatch_event(e_renderer_event_type::end_render, { }); }
		void setup_state() { dispatch_event(e_renderer_event_type::setup_state, { }); }
		void viewport_resize_begin() { dispatch_event(e_renderer_event_type::viewport_resize_begin, { }); }
		void viewport_resize_end() { dispatch_event(e_renderer_event_type::viewport_resize_end, { }); }
	} inline renderer_event_dispatcher{ };

	class i_renderer_event_listener : public utils::i_event_listener<e_renderer_event_type> {
	private:
		void process_event(e_renderer_event_type id, const std::unordered_map<std::string, std::any>& parameters) override {
			switch(id) {
				case e_renderer_event_type::create: { on_create(); } break;
				case e_renderer_event_type::destroy: { on_destroy(); } break;
				case e_renderer_event_type::begin_render: { on_begin_render(); } break;
				case e_renderer_event_type::end_render: { on_end_render(); } break;
				case e_renderer_event_type::setup_state: { on_setup_state(); } break;
				case e_renderer_event_type::viewport_resize_begin: { on_viewport_resize_begin(); } break;
				case e_renderer_event_type::viewport_resize_end: { on_viewport_resize_end(); } break;
			}
		}

	public:
		virtual void on_create() { }
		virtual void on_destroy() { }
		virtual void on_begin_render() { }
		virtual void on_end_render() { }
		virtual void on_setup_state() { }
		virtual void on_viewport_resize_begin() { }
		virtual void on_viewport_resize_end() { }
	};
}