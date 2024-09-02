#pragma once
#include "object.h"
#include "renderer-events.h"
#include "../renderer/renderer.h"

namespace ntl::render::backend {
    enum class e_stencil_operation {
        set,
        set_inverse,
        intersect,

        end_write
    };

    class i_stencil_buffer : protected i_renderer_event_listener {
    public:
        i_stencil_buffer() {
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::viewport_resize_begin, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::viewport_resize_end, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::destroy, this);
        }

        virtual ~i_stencil_buffer() {
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::viewport_resize_begin, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::viewport_resize_end, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::destroy, this);
        }

    public:
        virtual void* get_buffer() = 0;

    public:
        virtual void create() = 0;
        virtual void destroy() = 0;

        virtual void clear() = 0;

        virtual void set_test(bool test) = 0;
        virtual void set_operation(e_stencil_operation operation) = 0;

    public:
        virtual bool empty() const = 0;

    protected:
        virtual void resize_begin() {
            destroy();
        }

        virtual void resize_end() {
            create();
        }

    protected:
        virtual void on_create() override { create(); }
        virtual void on_destroy() override { destroy(); }
        virtual void on_viewport_resize_begin() override { resize_begin(); }
        virtual void on_viewport_resize_end() override { resize_end(); }
    }; inline std::unique_ptr<i_stencil_buffer> stencil_buffer{ };
}
