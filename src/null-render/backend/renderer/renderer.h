#pragma once
#include "../internal/geometry-buffer.h"

namespace ntl::render {
    namespace shared {
        inline vec2_t<int> viewport{ };
        inline std::uint32_t msaa_quality{ 8 };
    }

    namespace backend {
        class i_renderer {
        protected:
            matrix4x4_t matrix{ };
            vec2_t<float> translation{ };
            float depth{ };

        public:
            i_renderer() { }
            virtual ~i_renderer() { }

        protected:
            virtual void update_uniforms();

        public:
            inline void set_matrix(const matrix4x4_t& _matrix) { matrix = _matrix; }
            inline void update_matrix(const matrix4x4_t& _matrix) { set_matrix(_matrix); update_uniforms(); }
            inline const matrix4x4_t& get_matrix() const { return matrix; }

            inline void set_translation(const vec2_t<float>& _translation) { translation = _translation; }
            inline void update_translation(const vec2_t<float>& _translation) { set_translation(_translation); update_uniforms(); }
            inline const vec2_t<float>& get_translation() { return translation; }

            inline void set_depth(float _depth) { depth = _depth; }
            inline void update_depth(float _depth) { set_depth(_depth); update_uniforms(); }
            inline float get_depth() { return depth; }

            //@note: opengl is crap, there may be an easier way to do this, but for now it will be like this
            constexpr virtual bool framebuffer_uvs_flipped() = 0;

        public:
            virtual matrix4x4_t get_projection_matrix() const = 0;

            virtual void set_texture(void* texture) = 0;

            virtual void set_clip(const rect_t<float>& rect) = 0;
            virtual void draw_geometry(e_topology topology, size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) = 0;

            virtual void* create_texture(const vec2_t<float>& size, void* data) = 0;
            virtual void destroy_texture(void* texture) = 0;

            virtual std::unique_ptr<std::uint8_t[]> premultiply_texture_color(const vec2_t<float>& size, std::uint8_t* data);

        protected:
            virtual void create_internal_objects() { }
            virtual void destroy_internal_objects() { }

        public:
            void create_objects();
            void destroy_objects();

            void begin_resize_viewport(const vec2_t<int>& new_viewport);
            void end_resize_viewport();

        public:
            void create_atlases();
            void destroy_atlases();

        public:
            virtual bool empty() const = 0;
        }; inline std::unique_ptr<i_renderer> renderer{ };
    }
}