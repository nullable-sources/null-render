#pragma once
#include "../../graphic/draw-list/draw-list.h"
#include "../renderer/renderer.h"

namespace ntl::render::backend {
    class c_renderer_pipeline {
    public:
        i_frame_buffer* rendering_buffer{ }, *intermediate_buffer{ };
        std::vector<c_draw_list*> flushing_draw_lists{ };

        bool use_rendering_buffer{ false }, use_intermediate_buffer{ true };
        bool clear_rendering_buffer{ false }, clear_intermediate_buffer{ true };
        bool blit_rendering_buffer{ false }, blit_intermediate_buffer{ true };

        bool store_state_pipeline{ true };
        bool setup_state_pipeline{ true };

    public:
        c_renderer_pipeline() { }
        virtual ~c_renderer_pipeline() { }

    public:
        template <typename self_t>
        auto&& set_rendering_buffer(this self_t&& self, i_frame_buffer* frame_buffer) {
            self.rendering_buffer = frame_buffer;
            return self;
        }

        template <typename self_t>
        auto&& set_intermediate_buffer(this self_t&& self, i_frame_buffer* frame_buffer) {
            self.intermediate_buffer = frame_buffer;
            return self;
        }

        template <typename self_t>
        auto&& add_flush_draw_list(this self_t&& self, c_draw_list* draw_list) {
            self.flushing_draw_lists.push_back(draw_list);
            return self;
        }

        template <typename self_t>
        auto&& set_rendering_buffer_using(this self_t&& self, bool rendering_buffer_using) {
            self.use_rendering_buffer = rendering_buffer_using;
            return self;
        }

        template <typename self_t>
        auto&& set_rendering_buffer_clear(this self_t&& self, bool rendering_buffer_clear) {
            self.clear_rendering_buffer = rendering_buffer_clear;
            return self;
        }

        template <typename self_t>
        auto&& set_rendering_buffer_blit(this self_t&& self, bool rendering_buffer_blit) {
            self.blit_rendering_buffer = rendering_buffer_blit;
            return self;
        }

        template <typename self_t>
        auto&& set_intermediate_buffer_using(this self_t&& self, bool intermediate_buffer_using) {
            self.use_intermediate_buffer = intermediate_buffer_using;
            return self;
        }

        template <typename self_t>
        auto&& set_intermediate_buffer_clear(this self_t&& self, bool intermediate_buffer_clear) {
            self.clear_intermediate_buffer = intermediate_buffer_clear;
            return self;
        }

        template <typename self_t>
        auto&& set_intermediate_buffer_blit(this self_t&& self, bool intermediate_buffer_blit) {
            self.blit_intermediate_buffer = intermediate_buffer_blit;
            return self;
        }

        template <typename self_t>
        auto&& set_state_pipeline_store(this self_t&& self, bool state_pipeline_store) {
            self.store_state_pipeline = state_pipeline_store;
            return self;
        }

        template <typename self_t>
        auto&& set_state_pipeline_setup(this self_t&& self, bool state_pipeline_setup) {
            self.setup_state_pipeline = state_pipeline_setup;
            return self;
        }

    public:
        virtual void begin();
        virtual void flush();
        virtual void end();

        virtual void process() { begin(); flush(); end(); }
    }; inline std::unique_ptr<c_renderer_pipeline> renderer_pipeline{ };
}