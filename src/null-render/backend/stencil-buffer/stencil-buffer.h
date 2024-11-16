#pragma once
#include "../internal/object.h"
#include "../internal/renderer-events.h"
#include "../renderer/renderer.h"
#include "../state-pipeline/blend-state.h"
#include "../state-pipeline/stencil-state.h"

namespace ntl::render::backend {
    enum class e_stencil_operation {
        set,
        set_inverse,
        intersect,

        end_write
    };

    class i_stencil_buffer : protected i_renderer_event_listener {
    private:
        bool already_testing{ };

    public:
        std::unique_ptr<backend::i_blend_state> disable_write_blend{ };
        std::unique_ptr<backend::i_stencil_state> stencil_test_state{ }, stencil_set_write_state{ }, stencil_intersect_write_state{ };
        int stencil_ref{ -1 };

    public:
        i_stencil_buffer();

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

    public:
        void set_test(bool test);
        void set_operation(e_stencil_operation operation);

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
