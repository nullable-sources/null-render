#include "renderer/renderer.h"
#include "wrapper/opengl3.h"

namespace null::render::opengl3 {
    matrix4x4_t c_renderer::get_projection_matrix() const {
        return matrix4x4_t::projection_ortho(0.f, shared::viewport.x, shared::viewport.y, 0.f, -10000.f, 10000.f);
    }

    void c_renderer::set_texture(void* texture) {
        opengl::bind_texture(opengl::e_texture_2d, (std::uint32_t)texture);
    }
    
    void c_renderer::set_clip(const rect_t<float>& rect) {
        const vec2_t size{ rect.size() };
        opengl::scissor(rect.min.x, shared::viewport.y - rect.max.y, size.x, size.y);
    }
    
    void c_renderer::draw_geometry(size_t vertex_count, size_t index_count, size_t vertex_offset, size_t index_offset) {
        opengl::draw_elements_base_vertex(opengl::e_triangles, index_count, opengl::e_unsigned_int, (void*)(std::intptr_t)(index_offset * sizeof(std::uint32_t)), vertex_offset);
    }

    void* c_renderer::create_texture(const vec2_t<float>& size, void* data) {
        std::uint32_t texture{ };
        opengl::gen_textures(1, &texture);
        opengl::bind_texture(opengl::e_texture_2d, texture);
        opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_min_filter, opengl::e_linear);
        opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_mag_filter, opengl::e_linear);
        opengl::tex_image2d(opengl::e_texture_2d, 0, opengl::e_rgba, size.x, size.y, 0, opengl::e_rgba, opengl::e_unsigned_byte, data);
        opengl::bind_texture(opengl::e_texture_2d, 0);

        if(!texture) {
            utils::logger(utils::e_log_type::warning, "Failed to create texture.");
            return nullptr;
        }

        return (void*)texture;
    }
    
    void c_renderer::destroy_texture(void* texture) {
        if(!texture) {
            utils::logger(utils::e_log_type::warning, "It is impossible to destroy the texture because it is empty.");
            return;
        }

        opengl::delete_textures(1, (std::uint32_t*)&texture);
        texture = nullptr;
    }

    void c_renderer::setup_state() {
        opengl::enable(opengl::e_blend);
        opengl::blend_equation(opengl::e_func_add);
        opengl::blend_func_separate(opengl::e_src_alpha, opengl::e_one_minus_src_alpha, 1, opengl::e_one_minus_src_alpha);
        opengl::disable(opengl::e_cull_face);
        opengl::disable(opengl::e_depth_test);
        opengl::disable(opengl::e_stencil_test);
        opengl::enable(opengl::e_scissor_test);
        opengl::disable(opengl::e_primitive_restart);

        //opengl::viewport(0.f, shared::viewport.y, shared::viewport.x, -shared::viewport.y);

        set_clip({ { 0 }, shared::viewport });
        set_matrix(get_projection_matrix());
        backend::object_event_dispatcher.setup_state();

        backend::mesh->set();
    }

    void c_renderer::save_state() {
        opengl::get_integerv(opengl::e_draw_framebuffer_binding, (int*)&saved_state.draw_fbo);
        opengl::get_integerv(opengl::e_read_framebuffer_binding, (int*)&saved_state.read_fbo);
        opengl::get_integerv(opengl::e_renderbuffer_binding, (int*)&saved_state.rbo);
        opengl::get_integerv(opengl::e_active_texture, (int*)&saved_state.active_texture);
        opengl::active_texture(opengl::e_texture0);
        opengl::get_integerv(opengl::e_current_program, (int*)&saved_state.program);
        opengl::get_integerv(opengl::e_texture_binding_2d, (int*)&saved_state.texture);
        opengl::get_integerv(opengl::e_sampler_binding, (int*)&saved_state.sampler);
        opengl::get_integerv(opengl::e_array_buffer_binding, (int*)&saved_state.array_buffer);
        opengl::get_integerv(opengl::e_vertex_array_binding, (int*)&saved_state.vertex_array_object);

        opengl::get_integerv(opengl::e_viewport, saved_state.viewport);
        opengl::get_integerv(opengl::e_scissor_box, saved_state.scissor_box);
        opengl::get_integerv(opengl::e_blend_src_rgb, (int*)&saved_state.blend_src_rgb);
        opengl::get_integerv(opengl::e_blend_dst_rgb, (int*)&saved_state.blend_dst_rgb);
        opengl::get_integerv(opengl::e_blend_src_alpha, (int*)&saved_state.blend_src_alpha);
        opengl::get_integerv(opengl::e_blend_dst_alpha, (int*)&saved_state.blend_dst_alpha);
        opengl::get_integerv(opengl::e_blend_equation_rgb, (int*)&saved_state.blend_equation_rgb);
        opengl::get_integerv(opengl::e_blend_equation_alpha, (int*)&saved_state.blend_equation_alpha);
        saved_state.enable_blend = opengl::is_enabled(opengl::e_blend);
        saved_state.enable_cull_face = opengl::is_enabled(opengl::e_cull_face);
        saved_state.enable_depth_test = opengl::is_enabled(opengl::e_depth_test);
        saved_state.enable_stencil_test = opengl::is_enabled(opengl::e_stencil_test);
        saved_state.enable_scissor_test = opengl::is_enabled(opengl::e_scissor_test);
        saved_state.enable_primitive_restart = opengl::is_enabled(opengl::e_primitive_restart);
    }
    
    void c_renderer::restore_state() {
        opengl::bind_framebuffer(opengl::e_read_framebuffer, saved_state.read_fbo);
        opengl::bind_framebuffer(opengl::e_draw_framebuffer, saved_state.draw_fbo);
        opengl::bind_renderbuffer(opengl::e_renderbuffer, saved_state.rbo);
        opengl::use_program(saved_state.program);
        opengl::bind_texture(opengl::e_texture_2d, saved_state.texture);
        opengl::bind_sampler(0, saved_state.sampler);
        opengl::active_texture(saved_state.active_texture);
        opengl::bind_vertex_array(saved_state.vertex_array_object);
        opengl::bind_buffer(opengl::e_array_buffer, saved_state.array_buffer);
        opengl::blend_equation_separate(saved_state.blend_equation_rgb, saved_state.blend_equation_alpha);
        opengl::blend_func_separate(saved_state.blend_src_rgb, saved_state.blend_dst_rgb, saved_state.blend_src_alpha, saved_state.blend_dst_alpha);
        if(saved_state.enable_blend) opengl::enable(opengl::e_blend); else opengl::disable(opengl::e_blend);
        if(saved_state.enable_cull_face) opengl::enable(opengl::e_cull_face); else opengl::disable(opengl::e_cull_face);
        if(saved_state.enable_depth_test) opengl::enable(opengl::e_depth_test); else opengl::disable(opengl::e_depth_test);
        if(saved_state.enable_stencil_test) opengl::enable(opengl::e_stencil_test); else opengl::disable(opengl::e_stencil_test);
        if(saved_state.enable_scissor_test) opengl::enable(opengl::e_scissor_test); else opengl::disable(opengl::e_scissor_test);
        if(saved_state.enable_primitive_restart) opengl::enable(opengl::e_primitive_restart); else opengl::disable(opengl::e_primitive_restart);
        opengl::viewport(saved_state.viewport[0], saved_state.viewport[1], saved_state.viewport[2], saved_state.viewport[3]);
        opengl::scissor(saved_state.scissor_box[0], saved_state.scissor_box[1], saved_state.scissor_box[2], saved_state.scissor_box[3]);
    }
}