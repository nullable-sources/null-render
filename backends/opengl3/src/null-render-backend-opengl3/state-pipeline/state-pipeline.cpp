#include "../wrapper/opengl3.h"
#include "state-pipeline.h"

namespace ntl::render::opengl3 {
    void c_state_pipeline::setup_state() {
        opengl::blend_equation(opengl::e_func_add);
        opengl::enable(opengl::e_depth_test);
        opengl::depth_mask(true);
        opengl::depth_func(opengl::e_lequal);

        opengl::disable(opengl::e_cull_face);
        opengl::disable(opengl::e_primitive_restart);

        i_state_pipeline::setup_state();
    }

    void c_state_pipeline::save_state() {
        opengl::get_integerv(opengl::e_draw_framebuffer_binding, (int*)&saved_state.draw_fbo);
        opengl::get_integerv(opengl::e_read_framebuffer_binding, (int*)&saved_state.read_fbo);
        opengl::get_integerv(opengl::e_renderbuffer_binding, (int*)&saved_state.rbo);
        opengl::get_integerv(opengl::e_active_texture, (int*)&saved_state.active_texture);
        opengl::active_texture(opengl::e_texture0);
        opengl::get_integerv(opengl::e_current_program, (int*)&saved_state.program);
        opengl::get_integerv(opengl::e_texture_binding_2d, (int*)&saved_state.texture);
        opengl::get_integerv(opengl::e_sampler_binding, (int*)&saved_state.sampler);
        opengl::get_integerv(opengl::e_array_buffer_binding, (int*)&saved_state.array_buffer);
        opengl::get_integerv(opengl::e_element_array_buffer, (int*)&saved_state.element_array_buffer);
        opengl::get_integerv(opengl::e_vertex_array_binding, (int*)&saved_state.vertex_array_object);

        opengl::get_integerv(opengl::e_viewport, saved_state.viewport);
        opengl::get_integerv(opengl::e_scissor_box, saved_state.scissor_box);
        saved_state.enable_cull_face = opengl::is_enabled(opengl::e_cull_face);
        saved_state.enable_depth_test = opengl::is_enabled(opengl::e_depth_test);
        saved_state.depth_mask = opengl::is_enabled(opengl::e_depth_writemask);
        opengl::get_integerv(opengl::e_depth_func, &saved_state.depth_func);
        saved_state.enable_primitive_restart = opengl::is_enabled(opengl::e_primitive_restart);
        saved_state.enable_stencil_test = opengl::is_enabled(opengl::e_stencil_test);

        opengl::get_integerv(opengl::e_stencil_clear_value, &saved_state.stencil_clear_value);

        opengl::get_integerv(opengl::e_stencil_func, &saved_state.stencil_front.func);
        opengl::get_integerv(opengl::e_stencil_ref, &saved_state.stencil_front.ref);
        opengl::get_integerv(opengl::e_stencil_value_mask, &saved_state.stencil_front.value_mask);
        opengl::get_integerv(opengl::e_stencil_writemask, &saved_state.stencil_front.writemask);
        opengl::get_integerv(opengl::e_stencil_fail, &saved_state.stencil_front.fail);
        opengl::get_integerv(opengl::e_stencil_pass_depth_fail, &saved_state.stencil_front.pass_depth_fail);
        opengl::get_integerv(opengl::e_stencil_pass_depth_pass, &saved_state.stencil_front.pass_depth_pass);

        opengl::get_integerv(opengl::e_stencil_back_func, &saved_state.stencil_back.func);
        opengl::get_integerv(opengl::e_stencil_back_ref, &saved_state.stencil_back.ref);
        opengl::get_integerv(opengl::e_stencil_back_value_mask, &saved_state.stencil_back.value_mask);
        opengl::get_integerv(opengl::e_stencil_back_writemask, &saved_state.stencil_back.writemask);
        opengl::get_integerv(opengl::e_stencil_back_fail, &saved_state.stencil_back.fail);
        opengl::get_integerv(opengl::e_stencil_back_pass_depth_fail, &saved_state.stencil_back.pass_depth_fail);
        opengl::get_integerv(opengl::e_stencil_back_pass_depth_pass, &saved_state.stencil_back.pass_depth_pass);

        //@note: blend save
        saved_blend.enable_blend = opengl::is_enabled(opengl::e_blend);
        opengl::get_integerv(opengl::e_blend_src_rgb, (int*)&saved_blend.blend_src_rgb);
        opengl::get_integerv(opengl::e_blend_dst_rgb, (int*)&saved_blend.blend_dst_rgb);
        opengl::get_integerv(opengl::e_blend_src_alpha, (int*)&saved_blend.blend_src_alpha);
        opengl::get_integerv(opengl::e_blend_dst_alpha, (int*)&saved_blend.blend_dst_alpha);
        opengl::get_integerv(opengl::e_blend_equation_rgb, (int*)&saved_blend.blend_equation_rgb);
        opengl::get_integerv(opengl::e_blend_equation_alpha, (int*)&saved_blend.blend_equation_alpha);
        opengl::get_integerv(opengl::e_color_writemask, saved_blend.color_write);
        opengl::get_floatv(opengl::e_blend_color, saved_blend.blend_color);

        //@note: rasterizer save
        saved_rasterizer.enable_scissor_test = opengl::is_enabled(opengl::e_scissor_test);
        saved_rasterizer.enable_multisample = opengl::is_enabled(opengl::e_multisample);

        stored_frame_buffer->set_from_external((void*)saved_state.draw_fbo);
    }

    void c_state_pipeline::restore_state() {
        i_state_pipeline::restore_state();

        restore_framebuffer();
        restore_shader();
        restore_mesh();
        restore_texture();
        restore_rasterizer();
        restore_blend();
        restore_stencil();

        if(saved_state.enable_cull_face) opengl::enable(opengl::e_cull_face); else opengl::disable(opengl::e_cull_face);
        if(saved_state.enable_depth_test) opengl::enable(opengl::e_depth_test); else opengl::disable(opengl::e_depth_test);
        opengl::depth_mask(saved_state.depth_mask);
        opengl::depth_func(saved_state.depth_func);
        if(saved_state.enable_primitive_restart) opengl::enable(opengl::e_primitive_restart); else opengl::disable(opengl::e_primitive_restart);
        opengl::viewport(saved_state.viewport[0], saved_state.viewport[1], saved_state.viewport[2], saved_state.viewport[3]);
        opengl::scissor(saved_state.scissor_box[0], saved_state.scissor_box[1], saved_state.scissor_box[2], saved_state.scissor_box[3]);
    }

    void c_state_pipeline::restore_framebuffer() {
        opengl::bind_framebuffer(opengl::e_read_framebuffer, saved_state.read_fbo);
        opengl::bind_framebuffer(opengl::e_draw_framebuffer, saved_state.draw_fbo);
        opengl::bind_renderbuffer(opengl::e_renderbuffer, saved_state.rbo);
    }

    void c_state_pipeline::restore_mesh() {
        opengl::bind_vertex_array(saved_state.vertex_array_object);
        opengl::bind_buffer(opengl::e_array_buffer, saved_state.array_buffer);
        opengl::bind_buffer(opengl::e_element_array_buffer, saved_state.element_array_buffer);
    }

    void c_state_pipeline::restore_shader() {
        opengl::use_program(saved_state.program);
    }

    void c_state_pipeline::restore_texture() {
        opengl::bind_texture(opengl::e_texture_2d, saved_state.texture);
        opengl::bind_sampler(0, saved_state.sampler);
        opengl::active_texture(saved_state.active_texture);
    }

    void c_state_pipeline::restore_rasterizer() {
        if(saved_rasterizer.enable_scissor_test) opengl::enable(opengl::e_scissor_test); else opengl::disable(opengl::e_scissor_test);
        if(saved_rasterizer.enable_multisample) opengl::enable(opengl::e_multisample); else opengl::disable(opengl::e_multisample);
    }

    void c_state_pipeline::restore_blend() {
        if(saved_blend.enable_blend) opengl::enable(opengl::e_blend); else opengl::disable(opengl::e_blend);
        opengl::blend_equation_separate(saved_blend.blend_equation_rgb, saved_blend.blend_equation_alpha);
        opengl::blend_func_separate(
            saved_blend.blend_src_rgb, saved_blend.blend_dst_rgb,
            saved_blend.blend_src_alpha, saved_blend.blend_dst_alpha
        );
        opengl::color_mask(saved_blend.color_write[0], saved_blend.color_write[1], saved_blend.color_write[2], saved_blend.color_write[3]);
        opengl::blend_color(saved_blend.blend_color[0], saved_blend.blend_color[1], saved_blend.blend_color[2], saved_blend.blend_color[3]);
    }

    void c_state_pipeline::restore_stencil() {
        if(saved_state.enable_stencil_test) opengl::enable(opengl::e_stencil_test); else opengl::disable(opengl::e_stencil_test);

        opengl::clear_stencil(saved_state.stencil_clear_value);
        opengl::stencil_func_separate(opengl::e_front, saved_state.stencil_front.func, saved_state.stencil_front.ref, saved_state.stencil_front.value_mask);
        opengl::stencil_mask_separate(opengl::e_front, saved_state.stencil_front.writemask);
        opengl::stencil_op_separate(opengl::e_front, saved_state.stencil_front.fail, saved_state.stencil_front.pass_depth_fail, saved_state.stencil_front.pass_depth_pass);

        opengl::stencil_func_separate(opengl::e_back, saved_state.stencil_back.func, saved_state.stencil_back.ref, saved_state.stencil_back.value_mask);
        opengl::stencil_mask_separate(opengl::e_back, saved_state.stencil_back.writemask);
        opengl::stencil_op_separate(opengl::e_back, saved_state.stencil_back.fail, saved_state.stencil_back.pass_depth_fail, saved_state.stencil_back.pass_depth_pass);
    }
}