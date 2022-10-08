#include <null-renderer-opengl3.h>

namespace null::renderer {
    void render_draw_data(draw_data_t& _draw_data) {
        if(draw_data_t::screen_size <= 0)
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

        std::uint32_t vertex_array_object{ };
        opengl::gen_vertex_arrays(1, &vertex_array_object);

        setup_state(vertex_array_object);

        for(render::c_draw_list* draw_list : _draw_data.draw_lists) {
            std::vector<vertex_t> vertex_buffer{ };
            std::ranges::transform(draw_list->vtx_buffer, std::back_inserter(vertex_buffer), [](render::vertex_t& vtx) { return vertex_t{ vtx.pos, vtx.uv, vtx.color }; });

            opengl::buffer_data(opengl::e_array_buffer, (std::intptr_t)vertex_buffer.size() * (int)sizeof(vertex_t), (const void*)vertex_buffer.data(), opengl::e_stream_draw);
            opengl::buffer_data(opengl::e_element_array_buffer, (std::intptr_t)draw_list->idx_buffer.size() * (int)sizeof(unsigned short), (const void*)draw_list->idx_buffer.data(), opengl::e_stream_draw);

            for(render::c_draw_list::cmd_t& cmd : draw_list->cmd_buffer) {
                if(cmd.callbacks.have_callback(render::e_cmd_callbacks::render_draw_data) && std::any_cast<bool>(cmd.callbacks.call<bool(render::c_draw_list::cmd_t*)>(render::e_cmd_callbacks::render_draw_data, &cmd))) {
                    setup_state(vertex_array_object);
                    continue;
                }

                if(cmd.clip_rect.min < draw_data_t::screen_size && cmd.clip_rect.max.x >= 0.0f && cmd.clip_rect.max.y >= 0.0f) {
                    opengl::scissor((int)cmd.clip_rect.min.x, (int)(draw_data_t::screen_size.y - cmd.clip_rect.max.y), (int)(cmd.clip_rect.size().x), (int)(cmd.clip_rect.size().y));

                    opengl::bind_texture(opengl::e_texture_2d, (std::uint32_t)cmd.texture);
                    opengl::draw_elements_base_vertex(opengl::e_triangles, cmd.element_count, sizeof(std::uint16_t) == 2 ? opengl::e_unsigned_short : opengl::e_unsigned_int, (void*)(std::intptr_t)(cmd.idx_offset * sizeof(std::uint16_t)), cmd.vtx_offset);
                    opengl::draw_elements(opengl::e_triangles, cmd.element_count, sizeof(std::uint16_t) == 2 ? opengl::e_unsigned_short : opengl::e_unsigned_int, (void*)(std::intptr_t)(cmd.idx_offset * sizeof(std::uint16_t)));
                }
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

    void setup_state(std::uint32_t vertex_array_object) {
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

        opengl::viewport(0, 0, draw_data_t::screen_size.x, draw_data_t::screen_size.y);
        matrix4x4_t ortho{ {
            { 2.f / draw_data_t::screen_size.x,0.f,                                     0.f,    0.f },
            { 0.f,                              2.f / (-draw_data_t::screen_size.y),    0.f,    0.f },
            { 0.f,                              0.f,                                    -1.f,   0.f },
            { -1.f,                             1.f,                                    0.f,    1.f }
            } };
        opengl::use_program(shader_program);
        opengl::uniform1i(attribute_texture, 0);
        opengl::uniform_matrix4fv(attribute_proj_mtx, 1, false, &ortho.matrix[0][0]);

        opengl::bind_vertex_array(vertex_array_object);

        opengl::bind_buffer(opengl::e_array_buffer, vbo_handle);
        opengl::bind_buffer(opengl::e_element_array_buffer, elements_handle);
        opengl::enable_vertex_attrib_array(attribute_position);
        opengl::enable_vertex_attrib_array(attribute_uv);
        opengl::enable_vertex_attrib_array(attribute_color);
        opengl::vertex_attrib_pointer(attribute_position, 2, opengl::e_float, false, sizeof(vertex_t), (void*)offsetof(vertex_t, pos));
        opengl::vertex_attrib_pointer(attribute_uv, 2, opengl::e_float, false, sizeof(vertex_t), (void*)offsetof(vertex_t, uv));
        opengl::vertex_attrib_pointer(attribute_color, 4, opengl::e_unsigned_byte, true, sizeof(vertex_t), (void*)offsetof(vertex_t, color));
    }

    bool create_fonts_texture() {
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

    void destroy_fonts_texture() {
        if(font_texture) {
            opengl::delete_textures(1, &font_texture);
            render::atlas.texture.data = nullptr;
            font_texture = 0;
        }
    }

    bool create_device_objects() {
        int last_texture, last_array_buffer;
        opengl::get_integerv(opengl::e_texture_binding_2d, &last_texture);
        opengl::get_integerv(opengl::e_array_buffer_binding, &last_array_buffer);
        int last_vertex_array;
        opengl::get_integerv(opengl::e_vertex_array_binding, &last_vertex_array);

        const char* vertex_source{
            #include <shaders/vertex.glsl>
        };
        vertex_shader = opengl::create_shader(opengl::e_vertex_shader);
        opengl::shader_source(vertex_shader, 1, &vertex_source, nullptr);
        opengl::compile_shader(vertex_shader);

        const char* fragment_source{
            #include <shaders/fragment.glsl>
        };
        frag_shader = opengl::create_shader(opengl::e_fragment_shader);
        opengl::shader_source(frag_shader, 1, &fragment_source, nullptr);
        opengl::compile_shader(frag_shader);

        shader_program = opengl::create_program();
        opengl::attach_shader(shader_program, vertex_shader);
        opengl::attach_shader(shader_program, frag_shader);
        opengl::link_program(shader_program);

        attribute_texture = opengl::get_uniform_location(shader_program, "sampler");
        attribute_proj_mtx = opengl::get_uniform_location(shader_program, "proj_mtx");
        attribute_position = opengl::get_attrib_location(shader_program, "position");
        attribute_uv = opengl::get_attrib_location(shader_program, "uv");
        attribute_color = opengl::get_attrib_location(shader_program, "color");

        opengl::gen_buffers(1, &vbo_handle);
        opengl::gen_buffers(1, &elements_handle);

        create_fonts_texture();

        opengl::bind_texture(opengl::e_texture_2d, last_texture);
        opengl::bind_buffer(opengl::e_array_buffer, last_array_buffer);
        opengl::bind_vertex_array(last_vertex_array);

        return true;
    }

    void destroy_device_objects() {
        if(vbo_handle) { opengl::delete_buffers(1, &vbo_handle); vbo_handle = 0; }
        if(elements_handle) { opengl::delete_buffers(1, &elements_handle); elements_handle = 0; }
        if(shader_program && vertex_shader) { opengl::detach_shader(shader_program, vertex_shader); }
        if(shader_program && frag_shader) { opengl::detach_shader(shader_program, frag_shader); }
        if(vertex_shader) { opengl::delete_shader(vertex_shader); vertex_shader = 0; }
        if(frag_shader) { opengl::delete_shader(frag_shader); frag_shader = 0; }
        if(shader_program) { opengl::delete_program(shader_program); shader_program = 0; }

        destroy_fonts_texture();
    }
}