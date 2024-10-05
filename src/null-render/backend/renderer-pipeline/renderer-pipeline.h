#pragma once
#include "../renderer/renderer.h"

namespace ntl::render::backend {
    class c_renderer_pipeline {
    public:
        i_frame_buffer* intermediate_buffer{ };

        bool flush_default_draw_list{ true };

        bool use_intermediate_buffer{ true };
        bool clear_intermediate_buffer{ true };
        bool blit_intermediate_buffer{ true };

        bool store_state_pipeline{ true };
        bool setup_state_pipeline{ true };

    public:
        c_renderer_pipeline() { }
        virtual ~c_renderer_pipeline() { }

    public:
        virtual void begin();
        virtual void end();

        virtual void process() { begin(); end(); }
    }; inline std::unique_ptr<c_renderer_pipeline> renderer_pipeline{ };
}