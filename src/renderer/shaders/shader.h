#pragma once
#include <renderer/renderer.h>

namespace null::render::impl::shaders{
    enum class e_event_type { create, destroy, begin_frame, end_frame, setup_state };
    class c_event_dispatcher : public utils::c_event_dispatcher<e_event_type> {
    public:
        void create() { dispatch_event(e_event_type::create, { }); }
        void destroy() { dispatch_event(e_event_type::destroy, { }); }
        void begin_frame() { dispatch_event(e_event_type::begin_frame, { }); }
        void end_frame() { dispatch_event(e_event_type::end_frame, { }); }
        void setup_state() { dispatch_event(e_event_type::setup_state, { }); }
    } inline event_dispatcher{ };

    class i_shader : public utils::i_event_listener<e_event_type> {
    public:
        i_shader() {
            event_dispatcher.attach_listener(e_event_type::create, this);
            event_dispatcher.attach_listener(e_event_type::destroy, this);
        }

        virtual ~i_shader() {
            event_dispatcher.detach_listener(e_event_type::create, this);
            event_dispatcher.detach_listener(e_event_type::destroy, this);
        }

    private:
        void process_event(const e_event_type& id, const std::unordered_map<std::string, std::any>& parameters) override {
            switch(id) {
                case e_event_type::create: { on_create(); } break;
                case e_event_type::destroy: { on_destroy(); } break;
                case e_event_type::begin_frame: { on_begin_frame(); } break;
                case e_event_type::end_frame: { on_end_frame(); } break;
                case e_event_type::setup_state: { on_setup_state(); } break;
            }
        }

    public:
        virtual void on_create() = 0;
        virtual void on_destroy() = 0;
        virtual void on_begin_frame() { }
        virtual void on_end_frame() { }
        virtual void on_setup_state() { }

    public:
        virtual void use() = 0;
        virtual bool empty() const = 0;
    };
}