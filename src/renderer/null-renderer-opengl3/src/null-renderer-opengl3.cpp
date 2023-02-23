#include <null-renderer-opengl3.h>
#include <shaders/general-texture/general-texture.h>
#include <shaders/general-color/general-color.h>

namespace null::renderer {
    void c_opengl3::render(const compiled_geometry_data_t& _compiled_geometry_data) {
        if(render::shared::viewport <= 0)
            return;

        std::uint32_t last_active_texture{ }; opengl::get_integerv(opengl::e_active_texture, (int*)&last_active_texture);
        opengl::active_texture(opengl::e_texture0);
        std::uint32_t last_program{ }; opengl::get_integerv(opengl::e_current_program, (int*)&last_program);
        std::uint32_t last_texture{ }; opengl::get_integerv(opengl::e_texture_binding_2d, (int*)&last_texture);
        std::uint32_t last_sampler{ }; opengl::get_integerv(opengl::e_sampler_binding, (int*)&last_sampler);
        std::uint32_t last_array_buffer{ }; opengl::get_integerv(opengl::e_array_buffer_binding, (int*)&last_array_buffer);
        std::uint32_t last_vertex_array_object{ }; opengl::get_integerv(opengl::e_vertex_array_binding, (int*)&last_vertex_array_object);
#ifdef opengl_polygon_mode
        int last_polygon_mode[2]{ }; opengl::get_integerv(opengl::e_polygon_mode, last_polygon_mode);
#endif
        int last_viewport[4]{ }; opengl::get_integerv(opengl::e_viewport, last_viewport);
        int last_scissor_box[4]{ }; opengl::get_integerv(opengl::e_scissor_box, last_scissor_box);
        std::uint32_t last_blend_src_rgb{ }; opengl::get_integerv(opengl::e_blend_src_rgb, (int*)&last_blend_src_rgb);
        std::uint32_t last_blend_dst_rgb{ }; opengl::get_integerv(opengl::e_blend_dst_rgb, (int*)&last_blend_dst_rgb);
        std::uint32_t last_blend_src_alpha{ }; opengl::get_integerv(opengl::e_blend_src_alpha, (int*)&last_blend_src_alpha);
        std::uint32_t last_blend_dst_alpha{ }; opengl::get_integerv(opengl::e_blend_dst_alpha, (int*)&last_blend_dst_alpha);
        std::uint32_t last_blend_equation_rgb{ }; opengl::get_integerv(opengl::e_blend_equation_rgb, (int*)&last_blend_equation_rgb);
        std::uint32_t last_blend_equation_alpha{ }; opengl::get_integerv(opengl::e_blend_equation_alpha, (int*)&last_blend_equation_alpha);
        bool last_enable_blend{ opengl::is_enabled(opengl::e_blend) };
        bool last_enable_cull_face{ opengl::is_enabled(opengl::e_cull_face) };
        bool last_enable_depth_test{ opengl::is_enabled(opengl::e_depth_test) };
        bool last_enable_stencil_test{ opengl::is_enabled(opengl::e_stencil_test) };
        bool last_enable_scissor_test{ opengl::is_enabled(opengl::e_scissor_test) };
        bool last_enable_primitive_restart{ opengl::is_enabled(opengl::e_primitive_restart) };

        opengl::gen_vertex_arrays(1, &vertex_array_object);

        setup_state();

        for(render::c_geometry_buffer* geometry_buffer : _compiled_geometry_data.geometry_buffers) {
            std::vector<vertex_t> vertex_buffer{ geometry_buffer->vtx_buffer | std::views::transform([](const render::vertex_t& vtx) { return vertex_t{ vtx.pos, vtx.uv, (std::uint32_t)((vtx.color.a & 0xff) << 24) | ((vtx.color.b & 0xff) << 16) | ((vtx.color.g & 0xff) << 8) | (vtx.color.r & 0xff) }; }) | std::ranges::to<std::vector>() };

            opengl::buffer_data(opengl::e_array_buffer, (std::intptr_t)geometry_buffer->vtx_buffer.size() * (int)sizeof(vertex_t), vertex_buffer.data(), opengl::e_stream_draw);
            opengl::buffer_data(opengl::e_element_array_buffer, (std::intptr_t)geometry_buffer->idx_buffer.size() * (int)sizeof(std::uint32_t), (const void*)geometry_buffer->idx_buffer.data(), opengl::e_stream_draw);

            std::uint32_t idx_offset{ };
            for(render::c_geometry_buffer::cmd_t& cmd : geometry_buffer->cmd_buffer) {
                if(auto& callback{ cmd.callbacks.at<render::e_cmd_callbacks::on_draw>() }; !callback.empty() && callback.call(cmd)) {
                    setup_state();
                    continue;
                }

                opengl::scissor((int)cmd.clip_rect.min.x, (int)(render::shared::viewport.y - cmd.clip_rect.max.y), (int)(cmd.clip_rect.size().x), (int)(cmd.clip_rect.size().y));

                if(!cmd.texture) render::shaders::general_color.use();
                else {
                    render::shaders::general_texture.use();
                    opengl::bind_texture(opengl::e_texture_2d, (std::uint32_t)cmd.texture);
                }

                opengl::draw_elements_base_vertex(opengl::e_triangles, cmd.element_count, opengl::e_unsigned_int, (void*)(std::intptr_t)(idx_offset * sizeof(std::uint32_t)), 0);
                idx_offset += cmd.element_count;
            }
        }

        opengl::delete_vertex_arrays(1, &vertex_array_object);

        opengl::use_program(last_program);
        opengl::bind_texture(opengl::e_texture_2d, last_texture);
        opengl::bind_sampler(0, last_sampler);
        opengl::active_texture(last_active_texture);
        opengl::bind_vertex_array(last_vertex_array_object);
        opengl::bind_buffer(opengl::e_array_buffer, last_array_buffer);
        opengl::blend_equation_separate(last_blend_equation_rgb, last_blend_equation_alpha);
        opengl::blend_func_separate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
        if(last_enable_blend) opengl::enable(opengl::e_blend); else opengl::disable(opengl::e_blend);
        if(last_enable_cull_face) opengl::enable(opengl::e_cull_face); else opengl::disable(opengl::e_cull_face);
        if(last_enable_depth_test) opengl::enable(opengl::e_depth_test); else opengl::disable(opengl::e_depth_test);
        if(last_enable_stencil_test) opengl::enable(opengl::e_stencil_test); else opengl::disable(opengl::e_stencil_test);
        if(last_enable_scissor_test) opengl::enable(opengl::e_scissor_test); else opengl::disable(opengl::e_scissor_test);
        if(last_enable_primitive_restart) opengl::enable(opengl::e_primitive_restart); else opengl::disable(opengl::e_primitive_restart);

#ifdef opengl_polygon_mode
        opengl::polygon_mode(opengl::e_front_and_back, (std::uint32_t)last_polygon_mode[0]);
#endif
        opengl::viewport(last_viewport[0], last_viewport[1], last_viewport[2], last_viewport[3]);
        opengl::scissor(last_scissor_box[0], last_scissor_box[1], last_scissor_box[2], last_scissor_box[3]);
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
#ifdef opengl_polygon_mode
        opengl::polygon_mode(opengl::e_front_and_back, opengl::e_fill);
#endif

        opengl::viewport(0.f, 0.f, render::shared::viewport.x, render::shared::viewport.y);
        
        opengl::bind_vertex_array(vertex_array_object);
       
        opengl::bind_buffer(opengl::e_array_buffer, vbo_handle);
        opengl::bind_buffer(opengl::e_element_array_buffer, elements_handle);
        
        render::shaders::general_color.setup_state();
        render::shaders::general_texture.setup_state();
    }

    void c_opengl3::create_objects() {
        int last_texture{ }, last_array_buffer{ };
        opengl::get_integerv(opengl::e_texture_binding_2d, &last_texture);
        opengl::get_integerv(opengl::e_array_buffer_binding, &last_array_buffer);
        int last_vertex_array{ };
        opengl::get_integerv(opengl::e_vertex_array_binding, &last_vertex_array);

        render::shaders::general_color.create();
        render::shaders::general_texture.create();

        opengl::gen_buffers(1, &vbo_handle);
        opengl::gen_buffers(1, &elements_handle);

        create_fonts_texture();

        opengl::bind_texture(opengl::e_texture_2d, last_texture);
        opengl::bind_buffer(opengl::e_array_buffer, last_array_buffer);
        opengl::bind_vertex_array(last_vertex_array);
    }

    void c_opengl3::destroy_objects() {
        if(vbo_handle) { opengl::delete_buffers(1, &vbo_handle); vbo_handle = 0; }
        if(elements_handle) { opengl::delete_buffers(1, &elements_handle); elements_handle = 0; }
        
        render::shaders::general_color.destroy();
        render::shaders::general_texture.destroy();

        destroy_fonts_texture();
    }

    bool c_opengl3::create_fonts_texture() {
        if(render::atlas.texture.pixels_alpha8.empty()) {
            if(render::atlas.configs.empty()) render::atlas.add_font_default();
            render::atlas.build();
        }

        render::atlas.texture.get_data_as_rgba32();

        int last_texture{ }; opengl::get_integerv(opengl::e_texture_binding_2d, &last_texture);
        opengl::gen_textures(1, &font_texture);
        opengl::bind_texture(opengl::e_texture_2d, font_texture);
        opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_min_filter, opengl::e_linear);
        opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_mag_filter, opengl::e_linear);
#ifdef opengl_unpack_row_length
        opengl::pixel_storei(opengl::e_unpack_row_length, 0);
#endif
        opengl::tex_image2d(opengl::e_texture_2d, 0, opengl::e_rgba, render::atlas.texture.size.x, render::atlas.texture.size.y, 0, opengl::e_rgba, opengl::e_unsigned_byte, (void*)render::atlas.texture.pixels_rgba32.data());

        render::atlas.texture.data = (void*)font_texture;

        opengl::bind_texture(opengl::e_texture_2d, last_texture);

        return true;
    }

    void c_opengl3::destroy_fonts_texture() {
        if(font_texture) {
            opengl::delete_textures(1, &font_texture);
            render::atlas.texture.data = nullptr;
            font_texture = 0;
        }
    }
}