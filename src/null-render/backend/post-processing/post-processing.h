#pragma once
#include "null-render/backend/internal/mesh.h"
#include "null-render/backend/internal/frame-buffer.h"
#include "null-render/backend/shaders/passthrough.h"
#include "../../graphic/commands/geometry/geometry.h"

namespace ntl::render::backend {
    class c_post_processing : protected i_renderer_event_listener {
    public:
        static constexpr rect_t<float> normalized_viewport = rect_t<float>(vec2_t<float>(-1.f, 1.f), vec2_t<float>(1.f, -1.f));
        static constexpr rect_t<float> normalized_uvs = rect_t<float>(0.f, 1.f);

    public:
        std::array<std::unique_ptr<i_frame_buffer>, 4> frame_buffers{ };
        std::unique_ptr<i_frame_buffer> transfer_buffer{ };

    protected:
        matrix4x4_t viewport_matrix{ };
        std::unique_ptr<c_mesh> mesh{ };
        std::shared_ptr<c_geometry_command> geometry_command{ };
        i_shader* custom_passthrough_shader{ };

        bool geometry_dirty{ };

    public:
        c_post_processing() {
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.attach_listener(e_renderer_event_type::viewport_resize_end, this);
        }

        virtual ~c_post_processing() {
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::create, this);
            renderer_event_dispatcher.detach_listener(e_renderer_event_type::viewport_resize_end, this);
        }

    public:
        void restore_passthrough_shader() { custom_passthrough_shader = nullptr; }
        void set_passthrough_shader(i_shader* shader) { custom_passthrough_shader = shader; }
        i_shader* get_passthrough_shader() { return custom_passthrough_shader ? custom_passthrough_shader : passthrough_shader.get(); }

    public:
        void initialize();

        rect_t<float> prepare_viewport_region(const rect_t<float>& screen_region);
        void generate_draw_geometry(c_geometry_command* command, const rect_t<float>& geometry_region, rect_t<float> uvs);

        //@note: each blit_buffer/draw_buffer method can take nullptr as a buffer argument,
        //       in this case the transfer buffer will be drawn
        //@note: blit methods use a passthrough shader, if you need to use your own shader,
        //       then use generate_geometry+draw_buffer_texture or draw_buffer/draw_buffer_region
        void blit_buffer_region(i_frame_buffer* buffer, const vec2_t<float>& uv_scaling);
        void blit_buffer_region(i_frame_buffer* buffer, const rect_t<float>& geometry_region, const rect_t<float>& uvs_region);
        void blit_buffer(i_frame_buffer* buffer);

        void draw_buffer_region(i_frame_buffer* buffer, const vec2_t<float>& uv_scaling);
        void draw_buffer_region(i_frame_buffer* buffer, const rect_t<float>& geometry_region, const rect_t<float>& uvs_region);
        void draw_buffer(i_frame_buffer* buffer);

        i_frame_buffer* at(size_t i) { return frame_buffers[i].get(); }

        void generate_geometry(const vec2_t<float>& uv_scaling);
        void generate_geometry(const rect_t<float>& geometry_region, const rect_t<float>& uvs_region);
        void draw_buffer_texture(i_frame_buffer* buffer);

        void to_transfer(i_frame_buffer* buffer);

    private:
        void* prepare_buffer_texture(i_frame_buffer* buffer);

        void generate_viewport_geometry();

        void on_create() override;
        void on_viewport_resize_end() override;
    }; inline std::unique_ptr<c_post_processing> post_processing{ };
}