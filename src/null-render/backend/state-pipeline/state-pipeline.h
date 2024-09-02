#pragma once
#include "../internal/mesh.h"
#include "../internal/frame-buffer.h"
#include "../internal/shader.h"

#include "rasterizer-state.h"
#include "blend-state.h"

namespace ntl::render::backend {
    class i_state_pipeline {
    public:
        template <typename object_t>
        class i_pipeline {
        public:
            std::vector<object_t*> stack{ };

        private:
            virtual void append() = 0;
            virtual void set_default() = 0;

        public:
            template <typename new_object_t> requires std::is_base_of_v<object_t, new_object_t> || std::is_same_v<object_t, new_object_t>
            void push(const std::unique_ptr<new_object_t>& object) { push(object.get()); }

            template <typename new_object_t> requires std::is_base_of_v<object_t, new_object_t> || std::is_same_v<object_t, new_object_t>
            void push(new_object_t* object) {
                stack.push_back(object);
                append();
            }

            void pop() {
                if(empty()) {
                    set_default();
                    return;
                }

                stack.pop_back();
                append_last();
            }

            void append_last() {
                if(empty()) set_default();
                else append();
            }

        public:
            object_t* current() { return empty() ? nullptr : stack.back(); }
            object_t* previous() { return stack.size() < 2 ? nullptr : *std::prev(stack.end(), 2); }

        public:
            bool empty() const { return stack.empty(); }
        };

        class c_framebuffers_pipeline : public i_pipeline<i_frame_buffer> {
        public:
            void append() override;
            void set_default() override;
        } framebuffers{ };

        class c_shaders_pipeline : public i_pipeline<i_shader> {
        public:
            void append() override;
            void set_default() override;
        } shaders{ };

        class c_meshes_pipeline : public i_pipeline<i_mesh> {
        public:
            void append() override;
            void set_default() override;
        } meshes{ };

        class c_textures_pipeline : public i_pipeline<void> {
        public:
            void append() override;
            void set_default() override;
        } textures{ };

        class c_rasterizers_pipeline : public i_pipeline<i_rasterizer_state> {
        public:
            void append() override;
            void set_default() override;
        } rasterizers{ };

        class c_blends_pipeline : public i_pipeline<i_blend_state> {
        public:
            void append() override;
            void set_default() override;
        } blends{ };

    public:
        virtual void setup_state();
        virtual void save_state() = 0;
        virtual void restore_state();

    public:
        virtual void restore_framebuffer() = 0;
        virtual void restore_shader() = 0;
        virtual void restore_mesh() = 0;
        virtual void restore_texture() = 0;
        virtual void restore_rasterizer() = 0;
        virtual void restore_blend() = 0;
    }; inline std::unique_ptr<i_state_pipeline> state_pipeline{ };
}
