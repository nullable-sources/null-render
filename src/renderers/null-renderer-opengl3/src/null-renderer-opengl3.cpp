#include <null-renderer-opengl3.h>

namespace null {
    namespace render {
        namespace opengl3 {
            void shutdown() {
                destroy_device_objects();
            }

            void begin_frame() {
                if(!shader_program)
                    create_device_objects();
            }

            void render_draw_data(c_draw_list::draw_data_t* _draw_data) {
                if(_draw_data->window_size <= 0)
                    return;

                GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
                glActiveTexture(GL_TEXTURE0);
                GLuint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&last_program);
                GLuint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture);
                GLuint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, (GLint*)&last_sampler);
                GLuint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&last_array_buffer);
                GLuint last_vertex_array_object; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&last_vertex_array_object);
#ifdef GL_POLYGON_MODE
                GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
#endif
                GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
                GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
                GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
                GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
                GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
                GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
                GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
                GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
                GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
                GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
                GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
                GLboolean last_enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
                GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
                GLboolean last_enable_primitive_restart = glIsEnabled(GL_PRIMITIVE_RESTART);

                GLuint vertex_array_object = 0;
                glGenVertexArrays(1, &vertex_array_object);

                setup_render_state(_draw_data, vertex_array_object);

                for(int n = 0; n < _draw_data->cmd_lists.size(); n++) {
                    c_draw_list* cmd_list{ _draw_data->cmd_lists[n] };

                    std::vector<vertex_t> vertex_buffer;
                    for(int i = 0; i < cmd_list->vtx_buffer.size(); i++) {
                        c_draw_list::vertex_t vtx = cmd_list->vtx_buffer[i];
                        vertex_buffer.push_back(vertex_t{ vtx.pos, vtx.uv, vtx.color });
                    }

                    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vertex_buffer.size() * (int)sizeof(vertex_t), (const GLvoid*)vertex_buffer.data(), GL_STREAM_DRAW);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->idx_buffer.size() * (int)sizeof(unsigned short), (const GLvoid*)cmd_list->idx_buffer.data(), GL_STREAM_DRAW);

                    for(int cmd_i = 0; cmd_i < cmd_list->cmd_buffer.size(); cmd_i++) {
                        c_draw_list::cmd_t* pcmd = &cmd_list->cmd_buffer[cmd_i];

                        if(pcmd->callbacks.have_callback(e_cmd_callbacks::render_draw_data) && std::any_cast<bool>(pcmd->callbacks.call<bool(c_draw_list::cmd_t*)>(e_cmd_callbacks::render_draw_data, pcmd))) {
                            setup_render_state(_draw_data, vertex_array_object);
                            continue;
                        }

                        if(pcmd->clip_rect.min < draw_data.window_size && pcmd->clip_rect.max.x >= 0.0f && pcmd->clip_rect.max.y >= 0.0f) {
                            glScissor((int)pcmd->clip_rect.min.x, (int)(draw_data.window_size.y - pcmd->clip_rect.max.y), (int)(pcmd->clip_rect.size().x), (int)(pcmd->clip_rect.size().y));

                            glBindTexture(GL_TEXTURE_2D, (GLuint)pcmd->texture_id);
                            glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)pcmd->element_count, sizeof(std::uint16_t) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(std::intptr_t)(pcmd->idx_offset * sizeof(std::uint16_t)), pcmd->vtx_offset);
                            glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->element_count, sizeof(std::uint16_t) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(std::intptr_t)(pcmd->idx_offset * sizeof(std::uint16_t)));
                        }
                    }
                }

                glDeleteVertexArrays(1, &vertex_array_object);

                glUseProgram(last_program);
                glBindTexture(GL_TEXTURE_2D, last_texture);
                glBindSampler(0, last_sampler);
                glActiveTexture(last_active_texture);
                glBindVertexArray(last_vertex_array_object);
                glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
                glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
                glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
                if(last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
                if(last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
                if(last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
                if(last_enable_stencil_test) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST);
                if(last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
                if(last_enable_primitive_restart) glEnable(GL_PRIMITIVE_RESTART); else glDisable(GL_PRIMITIVE_RESTART);

#ifdef GL_POLYGON_MODE
                glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
#endif
                glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
                glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
            }

            void setup_render_state(c_draw_list::draw_data_t* draw_data, GLuint vertex_array_object) {
                glEnable(GL_BLEND);
                glBlendEquation(GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                glDisable(GL_CULL_FACE);
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_STENCIL_TEST);
                glEnable(GL_SCISSOR_TEST);
                glDisable(GL_PRIMITIVE_RESTART);
#ifdef GL_POLYGON_MODE
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

                glViewport(0, 0, draw_data->window_size.x, draw_data->window_size.y);
                float L = draw_data->window_pos.x;
                float R = draw_data->window_pos.x + draw_data->window_size.x;
                float T = draw_data->window_pos.y;
                float B = draw_data->window_pos.y + draw_data->window_size.y;
                const float ortho_projection[4][4] =
                {
                    { 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
                    { 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
                    { 0.0f,         0.0f,        -1.0f,   0.0f },
                    { (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
                };
                glUseProgram(shader_program);
                glUniform1i(attribute_texture, 0);
                glUniformMatrix4fv(attribute_proj_mtx, 1, GL_FALSE, &ortho_projection[0][0]);

                glBindVertexArray(vertex_array_object);

                glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements_handle);
                glEnableVertexAttribArray(attribute_position);
                glEnableVertexAttribArray(attribute_uv);
                glEnableVertexAttribArray(attribute_color);
                glVertexAttribPointer(attribute_position, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offsetof(vertex_t, pos));
                glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offsetof(vertex_t, uv));
                glVertexAttribPointer(attribute_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex_t), (GLvoid*)offsetof(vertex_t, color));
            }

            bool create_fonts_texture() {
                if(atlas.texture.pixels_alpha8.empty()) {
                    if(atlas.configs.empty()) atlas.add_font_default();
                    atlas.build();
                }

                atlas.texture.get_data_as_rgba32();

                GLint last_texture;
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
                glGenTextures(1, &font_texture);
                glBindTexture(GL_TEXTURE_2D, font_texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas.texture.size.x, atlas.texture.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)atlas.texture.pixels_rgba32.data());

                atlas.texture.id = (void*)font_texture;

                glBindTexture(GL_TEXTURE_2D, last_texture);

                return true;
            }

            void destroy_fonts_texture() {
                if(font_texture) {
                    glDeleteTextures(1, &font_texture);
                    atlas.texture.id = nullptr;
                    font_texture = 0;
                }
            }

            bool create_device_objects() {
                GLint last_texture, last_array_buffer;
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
                GLint last_vertex_array;
                glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

                const char* vertex_source {
                    #include <shaders/vertex.glsl>
                };
                vertex_shader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
                glCompileShader(vertex_shader);

                const char* fragment_source {
                    #include <shaders/fragment.glsl>
                };
                frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(frag_shader, 1, &fragment_source, nullptr);
                glCompileShader(frag_shader);

                shader_program = glCreateProgram();
                glAttachShader(shader_program, vertex_shader);
                glAttachShader(shader_program, frag_shader);
                glLinkProgram(shader_program);

                attribute_texture = glGetUniformLocation(shader_program, "sampler");
                attribute_proj_mtx = glGetUniformLocation(shader_program, "proj_mtx");
                attribute_position = (GLuint)glGetAttribLocation(shader_program, "position");
                attribute_uv = (GLuint)glGetAttribLocation(shader_program, "uv");
                attribute_color = (GLuint)glGetAttribLocation(shader_program, "color");

                glGenBuffers(1, &vbo_handle);
                glGenBuffers(1, &elements_handle);

                create_fonts_texture();

                glBindTexture(GL_TEXTURE_2D, last_texture);
                glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
                glBindVertexArray(last_vertex_array);

                return true;
            }

            void destroy_device_objects() {
                if(vbo_handle) { glDeleteBuffers(1, &vbo_handle); vbo_handle = 0; }
                if(elements_handle) { glDeleteBuffers(1, &elements_handle); elements_handle = 0; }
                if(shader_program && vertex_shader) { glDetachShader(shader_program, vertex_shader); }
                if(shader_program && frag_shader) { glDetachShader(shader_program, frag_shader); }
                if(vertex_shader) { glDeleteShader(vertex_shader); vertex_shader = 0; }
                if(frag_shader) { glDeleteShader(frag_shader); frag_shader = 0; }
                if(shader_program) { glDeleteProgram(shader_program); shader_program = 0; }

                destroy_fonts_texture();
            }
        }
    }
}