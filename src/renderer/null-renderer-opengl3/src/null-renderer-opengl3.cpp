#include <null-renderer-opengl3.h>
#include <shaders/passthrough-texture/passthrough-texture.h>
#include <shaders/passthrough-color/passthrough-color.h>
#include <shaders/quad-gradient/quad-gradient.h>
#include <shaders/sdf/sdf.h>

namespace null::render {
    void c_opengl3::set_texture(void* texture) {
        opengl::bind_texture(opengl::e_texture_2d, (std::uint32_t)texture);
    }

    void c_opengl3::set_clip(const rect_t<float>& rect) {
        vec2_t size{ rect.size() };
        opengl::scissor(rect.min.x, shared::viewport.y - rect.max.y, size.x, size.y);
    }

    void c_opengl3::draw_geometry(const size_t& vertex_count, const size_t& index_count, const size_t& vertex_offset, const size_t& index_offset) {
        opengl::draw_elements_base_vertex(opengl::e_triangles, index_count, opengl::e_unsigned_int, (void*)(std::intptr_t)(index_offset * sizeof(std::uint32_t)), vertex_offset);
    }

    void c_opengl3::initialize() {
        render::impl::shaders::passthrough_color = std::make_unique<renderer::shaders::c_passthrough_color>();
        render::impl::shaders::passthrough_texture = std::make_unique<renderer::shaders::c_passthrough_texture>();

        render::impl::shaders::quad_gradient = std::make_unique<renderer::shaders::c_quad_gradient>();
        render::impl::shaders::sdf = std::make_unique<renderer::shaders::c_sdf>();
    }

    void c_opengl3::begin_render() {
        saved_state.save();
        opengl::gen_vertex_arrays(1, &vertex_array_object);

        setup_state();
        
        render::impl::shaders::passthrough_color->use();

        opengl::buffer_data(opengl::e_array_buffer, (std::intptr_t)geometry_buffer.vertex_buffer.size() * (int)sizeof(impl::vertex_t), geometry_buffer.vertex_buffer.data(), opengl::e_stream_draw);
        opengl::buffer_data(opengl::e_element_array_buffer, (std::intptr_t)geometry_buffer.index_buffer.size() * (int)sizeof(impl::index_t), (const void*)geometry_buffer.index_buffer.data(), opengl::e_stream_draw);
        
        background.handle();
        background.clear();
    }

    void c_opengl3::end_render() {
        geometry_buffer.clear();

        opengl::delete_vertex_arrays(1, &vertex_array_object);
        saved_state.restore();
    }

    void c_opengl3::setup_state() {
        opengl::enable(opengl::e_blend);
        opengl::blend_equation(opengl::e_func_add);
        opengl::blend_func_separate(opengl::e_src_alpha, opengl::e_one_minus_src_alpha, 1, opengl::e_one_minus_src_alpha);
        opengl::disable(opengl::e_cull_face);
        opengl::disable(opengl::e_depth_test);
        opengl::disable(opengl::e_stencil_test);
        opengl::enable(opengl::e_scissor_test);
        opengl::disable(opengl::e_primitive_restart);

        opengl::viewport(0.f, 0.f, render::shared::viewport.x, render::shared::viewport.y);
        
        opengl::bind_vertex_array(vertex_array_object);
       
        opengl::bind_buffer(opengl::e_array_buffer, vbo_handle);
        opengl::bind_buffer(opengl::e_element_array_buffer, elements_handle);

        set_clip({ { 0 }, render::shared::viewport });
        set_matrix(matrix4x4_t::project_ortho(0.f, render::shared::viewport.x, render::shared::viewport.y, 0.f, -10000.f, 10000.f));
        render::impl::shaders::event_dispatcher.setup_state();
    }

    void c_opengl3::create_objects() {
        render::impl::shaders::event_dispatcher.create();

        if(!vbo_handle) opengl::gen_buffers(1, &vbo_handle);
        if(!elements_handle) opengl::gen_buffers(1, &elements_handle);

        create_atlases();
    }

    void c_opengl3::destroy_objects() {
        if(vbo_handle) { opengl::delete_buffers(1, &vbo_handle); vbo_handle = 0; }
        if(elements_handle) { opengl::delete_buffers(1, &elements_handle); elements_handle = 0; }
        
        render::impl::shaders::event_dispatcher.destroy();

        destroy_atlases();
    }

    void* c_opengl3::create_texture(const vec2_t<float>& size, void* data) {
        std::uint32_t texture{ };
        opengl::gen_textures(1, &texture);
        opengl::bind_texture(opengl::e_texture_2d, texture);
        opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_min_filter, opengl::e_linear);
        opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_mag_filter, opengl::e_linear);
        opengl::tex_image2d(opengl::e_texture_2d, 0, opengl::e_rgba, size.x, size.y, 0, opengl::e_rgba, opengl::e_unsigned_byte, data);
        opengl::bind_texture(opengl::e_texture_2d, 0);

        if(!texture) {
            utils::logger.log(::utils::e_log_type::warning, "Failed to create texture.");
            return nullptr;
        }

        return (void*)texture;
    }

    void c_opengl3::destroy_texture(void* texture) {
        if(!texture) {
            utils::logger.log(::utils::e_log_type::warning, "It is impossible to destroy the texture because it is empty.");
            return;
        }

        opengl::delete_textures(1, (std::uint32_t*)&texture);
        texture = nullptr;
    }
}