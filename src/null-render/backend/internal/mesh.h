#pragma once
#include "geometry-buffer.h"
#include "object.h"
#include "renderer-events.h"

namespace null::render::backend {
    class i_mesh : protected i_renderer_event_listener, public i_usable_object {
    public:
        i_mesh() {
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::destroy, this);
        }

        virtual ~i_mesh() {
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::destroy, this);
        }

    public:
        virtual void create() = 0;
        virtual void destroy() = 0;
        virtual void compile() = 0;

    public:
        virtual void clear_geometry() = 0;

    protected:
        void on_create() override { create(); }
        void on_destroy() override { destroy(); }
    };

    class c_mesh : public i_mesh {
    public:
        c_geometry_buffer geometry_buffer{ };

    public:
        void clear_geometry() { geometry_buffer.clear(); }
    };
}
