#pragma once
#include "renderer-events.h"
#include "object.h"

namespace ntl::render::backend {
    class i_shader : public i_usable_object, protected i_renderer_event_listener {
    public:
        i_shader() {
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::destroy, this);
        }

        virtual ~i_shader() {
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::destroy, this);
        }

    public:
        virtual void create() { }
        virtual void destroy() { }

        virtual bool empty() const = 0;

    protected:
        virtual void on_create() override { create(); }
        virtual void on_destroy() override { destroy(); }
    };

    class i_compiled_shader_object : protected i_renderer_event_listener {
    public:
        i_compiled_shader_object() {
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::destroy, this);
        }

        virtual ~i_compiled_shader_object() {
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::destroy, this);
        }

    public:
        virtual void create() = 0;
        virtual void destroy() = 0;

        virtual bool empty() const = 0;

    protected:
        virtual void on_create() override { create(); }
        virtual void on_destroy() override { destroy(); }
    };
}