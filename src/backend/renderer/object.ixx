module;
#include <any>
#include <string>
#include <unordered_map>
export module null.render:backend.renderer.object;

import null.sdk;

export namespace null::render::backend {
    enum class e_object_event_type { create, destroy, begin_render, end_render, setup_state };
    class c_object_event_dispatcher : public utils::c_event_dispatcher<e_object_event_type> {
    public:
        void create() { dispatch_event(e_object_event_type::create, { }); }
        void destroy() { dispatch_event(e_object_event_type::destroy, { }); }
        void begin_render() { dispatch_event(e_object_event_type::begin_render, { }); }
        void end_render() { dispatch_event(e_object_event_type::end_render, { }); }
        void setup_state() { dispatch_event(e_object_event_type::setup_state, { }); }
    } object_event_dispatcher{ };

    class i_object : public utils::i_event_listener<e_object_event_type> {
    public:
        i_object() {
            object_event_dispatcher.attach_listener(e_object_event_type::create, this);
            object_event_dispatcher.attach_listener(e_object_event_type::destroy, this);
        }

        virtual ~i_object() {
            object_event_dispatcher.detach_listener(e_object_event_type::create, this);
            object_event_dispatcher.detach_listener(e_object_event_type::destroy, this);
        }

    private:
        void process_event(e_object_event_type id, const std::unordered_map<std::string, std::any>& parameters) override {
            switch(id) {
                case e_object_event_type::create: { on_create(); } break;
                case e_object_event_type::destroy: { on_destroy(); } break;
                case e_object_event_type::begin_render: { on_begin_render(); } break;
                case e_object_event_type::end_render: { on_end_render(); } break;
                case e_object_event_type::setup_state: { on_setup_state(); } break;
            }
        }

    public:
        virtual void on_create() = 0;
        virtual void on_destroy() = 0;
        virtual void on_begin_render() { }
        virtual void on_end_render() { }
        virtual void on_setup_state() { }
    };
}