#pragma once
#include "../renderer/renderer.h"
#include "object.h"
#include "renderer-events.h"

namespace ntl::render::backend {
    enum class e_frame_buffer_type {
        backbuffer,
        postprocessing,
        external
    };

    enum class e_frame_buffer_flags {
        none = 0,
        msaa = 1 << 0,
        depth_buffer = 1 << 1
    }; NULLSDK_ENUM_CREATE_BIT_OPERATORS(e_frame_buffer_flags, true);
    NULLSDK_ENUM_CREATE_CAST_OPERATOR(e_frame_buffer_flags, -);

    class i_frame_buffer : protected i_renderer_event_listener, public i_usable_object {
    public:
        vec2_t<int> size{ };
        e_frame_buffer_type type{ };
        e_frame_buffer_flags flags{ };

    public:
        i_frame_buffer(const vec2_t<int>& _size, e_frame_buffer_type _type, e_frame_buffer_flags _flags = { }) : size(_size), type(_type), flags(_flags) {
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::viewport_resize_begin, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::viewport_resize_end, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::destroy, this);
        }

        virtual ~i_frame_buffer() {
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::viewport_resize_begin, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::viewport_resize_end, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::destroy, this);
        }

    public:
        virtual void* get_surface() = 0;
        virtual void* get_texture() = 0;

        virtual void set_from_external(void* external_resource) = 0;

    public:
        virtual void create() = 0;
        virtual void destroy() = 0;

        virtual void clear() = 0;

        virtual void copy_from(i_frame_buffer* another_frame_buffer) = 0;
        virtual void blit_region_from(i_frame_buffer* another_frame_buffer, const vec2_t<int>& blit_offset, const rect_t<int>& region) = 0;
        virtual void copy_in_texture(void* texture, const rect_t<int>& region) = 0;

    public:
        virtual bool empty() const = 0;

    protected:
        virtual void resize_begin(const vec2_t<int>& new_size) {
            size = new_size;
            destroy();
        }

        virtual void resize_end() {
            create();
        }

    protected:
        virtual void on_create() override { create(); }
        virtual void on_destroy() override { destroy(); }
        virtual void on_viewport_resize_begin() override { resize_begin(shared::viewport); }
        virtual void on_viewport_resize_end() override { resize_end(); }
    }; inline std::unique_ptr<i_frame_buffer> msaa_buffer{ }, rendering_buffer{ };
}
