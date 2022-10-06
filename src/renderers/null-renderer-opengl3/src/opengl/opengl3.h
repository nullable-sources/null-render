#pragma once
#pragma comment(lib, "opengl32.lib");
#include <null-sdk.h>

namespace opengl {
	enum e_constants {
		e_accum = 0x0100,
		e_load = 0x0101,
		e_return = 0x0102,
		e_mult = 0x0103,
		e_add = 0x0104,
		e_current_bit = 0x00000001,
		e_point_bit = 0x00000002,
		e_line_bit = 0x00000004,
		e_polygon_bit = 0x00000008,
		e_polygon_stipple_bit = 0x00000010,
		e_pixel_mode_bit = 0x00000020,
		e_lighting_bit = 0x00000040,
		e_fog_bit = 0x00000080,
		e_accum_buffer_bit = 0x00000200,
		e_viewport_bit = 0x00000800,
		e_transform_bit = 0x00001000,
		e_enable_bit = 0x00002000,
		e_hint_bit = 0x00008000,
		e_eval_bit = 0x00010000,
		e_list_bit = 0x00020000,
		e_texture_bit = 0x00040000,
		e_scissor_bit = 0x00080000,
		e_all_attrib_bits = 0x000fffff,
		e_quad_strip = 0x0008,
		e_polygon = 0x0009,
		e_clip_plane0 = 0x3000,
		e_clip_plane1 = 0x3001,
		e_clip_plane2 = 0x3002,
		e_clip_plane3 = 0x3003,
		e_clip_plane4 = 0x3004,
		e_clip_plane5 = 0x3005,
		e_2_bytes = 0x1407,
		e_3_bytes = 0x1408,
		e_4_bytes = 0x1409,
		e_aux0 = 0x0409,
		e_aux1 = 0x040A,
		e_aux2 = 0x040B,
		e_aux3 = 0x040C,
		e_2d = 0x0600,
		e_3d = 0x0601,
		e_3d_color = 0x0602,
		e_3d_color_texture = 0x0603,
		e_4d_color_texture = 0x0604,
		e_pass_through_token = 0x0700,
		e_point_token = 0x0701,
		e_line_token = 0x0702,
		e_polygon_token = 0x0703,
		e_bitmap_token = 0x0704,
		e_draw_pixel_token = 0x0705,
		e_copy_pixel_token = 0x0706,
		e_line_reset_token = 0x0707,
		e_exp = 0x0800,
		e_exp2 = 0x0801,
		e_coeff = 0x0A00,
		e_order = 0x0A01,
		e_domain = 0x0A02,
		e_current_color = 0x0B00,
		e_current_index = 0x0B01,
		e_current_normal = 0x0B02,
		e_current_texture_coords = 0x0B03,
		e_current_raster_color = 0x0B04,
		e_current_raster_index = 0x0B05,
		e_current_raster_texture_coords = 0x0B06,
		e_current_raster_position = 0x0B07,
		e_current_raster_position_valid = 0x0B08,
		e_current_raster_distance = 0x0B09,
		e_point_smooth = 0x0B10,
		e_line_stipple = 0x0B24,
		e_line_stipple_pattern = 0x0B25,
		e_line_stipple_repeat = 0x0B26,
		e_list_mode = 0x0B30,
		e_max_list_nesting = 0x0B31,
		e_list_base = 0x0B32,
		e_list_index = 0x0B33,
		e_polygon_stipple = 0x0B42,
		e_edge_flag = 0x0B43,
		e_lighting = 0x0B50,
		e_light_model_local_viewer = 0x0B51,
		e_light_model_two_side = 0x0B52,
		e_light_model_ambient = 0x0B53,
		e_shade_model = 0x0B54,
		e_color_material_face = 0x0B55,
		e_color_material_parameter = 0x0B56,
		e_color_material = 0x0B57,
		e_fog = 0x0B60,
		e_fog_index = 0x0B61,
		e_fog_density = 0x0B62,
		e_fog_start = 0x0B63,
		e_fog_end = 0x0B64,
		e_fog_mode = 0x0B65,
		e_fog_color = 0x0B66,
		e_accum_clear_value = 0x0B80,
		e_matrix_mode = 0x0BA0,
		e_normalize = 0x0BA1,
		e_modelview_stack_depth = 0x0BA3,
		e_projection_stack_depth = 0x0BA4,
		e_texture_stack_depth = 0x0BA5,
		e_modelview_matrix = 0x0BA6,
		e_projection_matrix = 0x0BA7,
		e_texture_matrix = 0x0BA8,
		e_attrib_stack_depth = 0x0BB0,
		e_client_attrib_stack_depth = 0x0BB1,
		e_alpha_test = 0x0BC0,
		e_alpha_test_func = 0x0BC1,
		e_alpha_test_ref = 0x0BC2,
		e_index_logic_op = 0x0BF1,
		e_aux_buffers = 0x0C00,
		e_index_clear_value = 0x0C20,
		e_index_writemask = 0x0C21,
		e_index_mode = 0x0C30,
		e_rgba_mode = 0x0C31,
		e_render_mode = 0x0C40,
		e_perspective_correction_hint = 0x0C50,
		e_point_smooth_hint = 0x0C51,
		e_fog_hint = 0x0C54,
		e_texture_gen_s = 0x0C60,
		e_texture_gen_t = 0x0C61,
		e_texture_gen_r = 0x0C62,
		e_texture_gen_q = 0x0C63,
		e_pixel_map_i_to_i = 0x0C70,
		e_pixel_map_s_to_s = 0x0C71,
		e_pixel_map_i_to_r = 0x0C72,
		e_pixel_map_i_to_g = 0x0C73,
		e_pixel_map_i_to_b = 0x0C74,
		e_pixel_map_i_to_a = 0x0C75,
		e_pixel_map_r_to_r = 0x0C76,
		e_pixel_map_g_to_g = 0x0C77,
		e_pixel_map_b_to_b = 0x0C78,
		e_pixel_map_a_to_a = 0x0C79,
		e_pixel_map_i_to_i_size = 0x0CB0,
		e_pixel_map_s_to_s_size = 0x0CB1,
		e_pixel_map_i_to_r_size = 0x0CB2,
		e_pixel_map_i_to_g_size = 0x0CB3,
		e_pixel_map_i_to_b_size = 0x0CB4,
		e_pixel_map_i_to_a_size = 0x0CB5,
		e_pixel_map_r_to_r_size = 0x0CB6,
		e_pixel_map_g_to_g_size = 0x0CB7,
		e_pixel_map_b_to_b_size = 0x0CB8,
		e_pixel_map_a_to_a_size = 0x0CB9,
		e_map_color = 0x0D10,
		e_map_stencil = 0x0D11,
		e_index_shift = 0x0D12,
		e_index_offset = 0x0D13,
		e_red_scale = 0x0D14,
		e_red_bias = 0x0D15,
		e_zoom_x = 0x0D16,
		e_zoom_y = 0x0D17,
		e_green_scale = 0x0D18,
		e_green_bias = 0x0D19,
		e_blue_scale = 0x0D1A,
		e_blue_bias = 0x0D1B,
		e_alpha_scale = 0x0D1C,
		e_alpha_bias = 0x0D1D,
		e_depth_scale = 0x0D1E,
		e_depth_bias = 0x0D1F,
		e_max_eval_order = 0x0D30,
		e_max_lights = 0x0D31,
		e_max_clip_planes = 0x0D32,
		e_max_pixel_map_table = 0x0D34,
		e_max_attrib_stack_depth = 0x0D35,
		e_max_modelview_stack_depth = 0x0D36,
		e_max_name_stack_depth = 0x0D37,
		e_max_projection_stack_depth = 0x0D38,
		e_max_texture_stack_depth = 0x0D39,
		e_max_client_attrib_stack_depth = 0x0D3B,
		e_index_bits = 0x0D51,
		e_red_bits = 0x0D52,
		e_green_bits = 0x0D53,
		e_blue_bits = 0x0D54,
		e_alpha_bits = 0x0D55,
		e_depth_bits = 0x0D56,
		e_stencil_bits = 0x0D57,
		e_accum_red_bits = 0x0D58,
		e_accum_green_bits = 0x0D59,
		e_accum_blue_bits = 0x0D5A,
		e_accum_alpha_bits = 0x0D5B,
		e_name_stack_depth = 0x0D70,
		e_auto_normal = 0x0D80,
		e_map1_color_4 = 0x0D90,
		e_map1_index = 0x0D91,
		e_map1_normal = 0x0D92,
		e_map1_texture_coord_1 = 0x0D93,
		e_map1_texture_coord_2 = 0x0D94,
		e_map1_texture_coord_3 = 0x0D95,
		e_map1_texture_coord_4 = 0x0D96,
		e_map1_vertex_3 = 0x0D97,
		e_map1_vertex_4 = 0x0D98,
		e_map2_color_4 = 0x0DB0,
		e_map2_index = 0x0DB1,
		e_map2_normal = 0x0DB2,
		e_map2_texture_coord_1 = 0x0DB3,
		e_map2_texture_coord_2 = 0x0DB4,
		e_map2_texture_coord_3 = 0x0DB5,
		e_map2_texture_coord_4 = 0x0DB6,
		e_map2_vertex_3 = 0x0DB7,
		e_map2_vertex_4 = 0x0DB8,
		e_map1_grid_domain = 0x0DD0,
		e_map1_grid_segments = 0x0DD1,
		e_map2_grid_domain = 0x0DD2,
		e_map2_grid_segments = 0x0DD3,
		e_feedback_buffer_pointer = 0x0DF0,
		e_feedback_buffer_size = 0x0DF1,
		e_feedback_buffer_type = 0x0DF2,
		e_selection_buffer_pointer = 0x0DF3,
		e_selection_buffer_size = 0x0DF4,
		e_texture_border = 0x1005,
		e_light0 = 0x4000,
		e_light1 = 0x4001,
		e_light2 = 0x4002,
		e_light3 = 0x4003,
		e_light4 = 0x4004,
		e_light5 = 0x4005,
		e_light6 = 0x4006,
		e_light7 = 0x4007,
		e_ambient = 0x1200,
		e_diffuse = 0x1201,
		e_specular = 0x1202,
		e_position = 0x1203,
		e_spot_direction = 0x1204,
		e_spot_exponent = 0x1205,
		e_spot_cutoff = 0x1206,
		e_constant_attenuation = 0x1207,
		e_linear_attenuation = 0x1208,
		e_quadratic_attenuation = 0x1209,
		e_compile = 0x1300,
		e_compile_and_execute = 0x1301,
		e_emission = 0x1600,
		e_shininess = 0x1601,
		e_ambient_and_diffuse = 0x1602,
		e_color_indexes = 0x1603,
		e_modelview = 0x1700,
		e_projection = 0x1701,
		e_color_index = 0x1900,
		e_luminance = 0x1909,
		e_luminance_alpha = 0x190A,
		e_bitmap = 0x1A00,
		e_render = 0x1C00,
		e_feedback = 0x1C01,
		e_select = 0x1C02,
		e_flat = 0x1D00,
		e_smooth = 0x1D01,
		e_s = 0x2000,
		e_t = 0x2001,
		e_r = 0x2002,
		e_q = 0x2003,
		e_modulate = 0x2100,
		e_decal = 0x2101,
		e_texture_env_mode = 0x2200,
		e_texture_env_color = 0x2201,
		e_texture_env = 0x2300,
		e_eye_linear = 0x2400,
		e_object_linear = 0x2401,
		e_sphere_map = 0x2402,
		e_texture_gen_mode = 0x2500,
		e_object_plane = 0x2501,
		e_eye_plane = 0x2502,
		e_clamp = 0x2900,
		e_client_pixel_store_bit = 0x00000001,
		e_client_vertex_array_bit = 0x00000002,
		e_client_all_attrib_bits = 0xffffffff,
		e_alpha4 = 0x803B,
		e_alpha8 = 0x803C,
		e_alpha12 = 0x803D,
		e_alpha16 = 0x803E,
		e_luminance4 = 0x803F,
		e_luminance8 = 0x8040,
		e_luminance12 = 0x8041,
		e_luminance16 = 0x8042,
		e_luminance4_alpha4 = 0x8043,
		e_luminance6_alpha2 = 0x8044,
		e_luminance8_alpha8 = 0x8045,
		e_luminance12_alpha4 = 0x8046,
		e_luminance12_alpha12 = 0x8047,
		e_luminance16_alpha16 = 0x8048,
		e_intensity = 0x8049,
		e_intensity4 = 0x804A,
		e_intensity8 = 0x804B,
		e_intensity12 = 0x804C,
		e_intensity16 = 0x804D,
		e_texture_luminance_size = 0x8060,
		e_texture_intensity_size = 0x8061,
		e_texture_priority = 0x8066,
		e_texture_resident = 0x8067,
		e_normal_array = 0x8075,
		e_color_array = 0x8076,
		e_index_array = 0x8077,
		e_texture_coord_array = 0x8078,
		e_edge_flag_array = 0x8079,
		e_vertex_array_size = 0x807A,
		e_vertex_array_type = 0x807B,
		e_vertex_array_stride = 0x807C,
		e_normal_array_type = 0x807E,
		e_normal_array_stride = 0x807F,
		e_color_array_size = 0x8081,
		e_color_array_type = 0x8082,
		e_color_array_stride = 0x8083,
		e_index_array_type = 0x8085,
		e_index_array_stride = 0x8086,
		e_texture_coord_array_size = 0x8088,
		e_texture_coord_array_type = 0x8089,
		e_texture_coord_array_stride = 0x808A,
		e_edge_flag_array_stride = 0x808C,
		e_vertex_array_pointer = 0x808E,
		e_normal_array_pointer = 0x808F,
		e_color_array_pointer = 0x8090,
		e_index_array_pointer = 0x8091,
		e_texture_coord_array_pointer = 0x8092,
		e_edge_flag_array_pointer = 0x8093,
		e_v2f = 0x2A20,
		e_v3f = 0x2A21,
		e_c4ub_v2f = 0x2A22,
		e_c4ub_v3f = 0x2A23,
		e_c3f_v3f = 0x2A24,
		e_n3f_v3f = 0x2A25,
		e_c4f_n3f_v3f = 0x2A26,
		e_t2f_v3f = 0x2A27,
		e_t4f_v4f = 0x2A28,
		e_t2f_c4ub_v3f = 0x2A29,
		e_t2f_c3f_v3f = 0x2A2A,
		e_t2f_n3f_v3f = 0x2A2B,
		e_t2f_c4f_n3f_v3f = 0x2A2C,
		e_t4f_c4f_n3f_v4f = 0x2A2D,
		e_vertex_array_ext = 0x8074,
		e_normal_array_ext = 0x8075,
		e_color_array_ext = 0x8076,
		e_index_array_ext = 0x8077,
		e_texture_coord_array_ext = 0x8078,
		e_edge_flag_array_ext = 0x8079,
		e_vertex_array_size_ext = 0x807A,
		e_vertex_array_type_ext = 0x807B,
		e_vertex_array_stride_ext = 0x807C,
		e_vertex_array_count_ext = 0x807D,
		e_normal_array_type_ext = 0x807E,
		e_normal_array_stride_ext = 0x807F,
		e_normal_array_count_ext = 0x8080,
		e_color_array_size_ext = 0x8081,
		e_color_array_type_ext = 0x8082,
		e_color_array_stride_ext = 0x8083,
		e_color_array_count_ext = 0x8084,
		e_index_array_type_ext = 0x8085,
		e_index_array_stride_ext = 0x8086,
		e_index_array_count_ext = 0x8087,
		e_texture_coord_array_size_ext = 0x8088,
		e_texture_coord_array_type_ext = 0x8089,
		e_texture_coord_array_stride_ext = 0x808A,
		e_texture_coord_array_count_ext = 0x808B,
		e_edge_flag_array_stride_ext = 0x808C,
		e_edge_flag_array_count_ext = 0x808D,
		e_vertex_array_pointer_ext = 0x808E,
		e_normal_array_pointer_ext = 0x808F,
		e_color_array_pointer_ext = 0x8090,
		e_index_array_pointer_ext = 0x8091,
		e_texture_coord_array_pointer_ext = 0x8092,
		e_edge_flag_array_pointer_ext = 0x8093,
		e_bgr_ext = 0x80E0,
		e_bgra_ext = 0x80E1,
		e_color_table_format_ext = 0x80D8,
		e_color_table_width_ext = 0x80D9,
		e_color_table_red_size_ext = 0x80DA,
		e_color_table_green_size_ext = 0x80DB,
		e_color_table_blue_size_ext = 0x80DC,
		e_color_table_alpha_size_ext = 0x80DD,
		e_color_table_luminance_size_ext = 0x80DE,
		e_color_table_intensity_size_ext = 0x80DF,
		e_color_index1_ext = 0x80E2,
		e_color_index2_ext = 0x80E3,
		e_color_index4_ext = 0x80E4,
		e_color_index8_ext = 0x80E5,
		e_color_index12_ext = 0x80E6,
		e_color_index16_ext = 0x80E7,
		e_max_elements_vertices_win = 0x80E8,
		e_max_elements_indices_win = 0x80E9,
		e_phong_win = 0x80EA,
		e_phong_hint_win = 0x80EB,
		e_fog_specular_texture_win = 0x80EC,
		e_depth_buffer_bit = 0x00000100,
		e_stencil_buffer_bit = 0x00000400,
		e_color_buffer_bit = 0x00004000,
		e_points = 0x0000,
		e_lines = 0x0001,
		e_line_loop = 0x0002,
		e_line_strip = 0x0003,
		e_triangles = 0x0004,
		e_triangle_strip = 0x0005,
		e_triangle_fan = 0x0006,
		e_quads = 0x0007,
		e_never = 0x0200,
		e_less = 0x0201,
		e_equal = 0x0202,
		e_lequal = 0x0203,
		e_greater = 0x0204,
		e_notequal = 0x0205,
		e_gequal = 0x0206,
		e_always = 0x0207,
		e_src_color = 0x0300,
		e_one_minus_src_color = 0x0301,
		e_src_alpha = 0x0302,
		e_one_minus_src_alpha = 0x0303,
		e_dst_alpha = 0x0304,
		e_one_minus_dst_alpha = 0x0305,
		e_dst_color = 0x0306,
		e_one_minus_dst_color = 0x0307,
		e_src_alpha_saturate = 0x0308,
		e_front_left = 0x0400,
		e_front_right = 0x0401,
		e_back_left = 0x0402,
		e_back_right = 0x0403,
		e_front = 0x0404,
		e_back = 0x0405,
		e_left = 0x0406,
		e_right = 0x0407,
		e_front_and_back = 0x0408,
		e_invalid_enum = 0x0500,
		e_invalid_value = 0x0501,
		e_invalid_operation = 0x0502,
		e_out_of_memory = 0x0505,
		e_cw = 0x0900,
		e_ccw = 0x0901,
		e_point_size = 0x0B11,
		e_point_size_range = 0x0B12,
		e_point_size_granularity = 0x0B13,
		e_line_smooth = 0x0B20,
		e_line_width = 0x0B21,
		e_line_width_range = 0x0B22,
		e_line_width_granularity = 0x0B23,
		e_polygon_mode = 0x0B40,
		e_polygon_smooth = 0x0B41,
		e_cull_face = 0x0B44,
		e_cull_face_mode = 0x0B45,
		e_front_face = 0x0B46,
		e_depth_range = 0x0B70,
		e_depth_test = 0x0B71,
		e_depth_writemask = 0x0B72,
		e_depth_clear_value = 0x0B73,
		e_depth_func = 0x0B74,
		e_stencil_test = 0x0B90,
		e_stencil_clear_value = 0x0B91,
		e_stencil_func = 0x0B92,
		e_stencil_value_mask = 0x0B93,
		e_stencil_fail = 0x0B94,
		e_stencil_pass_depth_fail = 0x0B95,
		e_stencil_pass_depth_pass = 0x0B96,
		e_stencil_ref = 0x0B97,
		e_stencil_writemask = 0x0B98,
		e_viewport = 0x0BA2,
		e_dither = 0x0BD0,
		e_blend_dst = 0x0BE0,
		e_blend_src = 0x0BE1,
		e_blend = 0x0BE2,
		e_logic_op_mode = 0x0BF0,
		e_draw_buffer = 0x0C01,
		e_read_buffer = 0x0C02,
		e_scissor_box = 0x0C10,
		e_scissor_test = 0x0C11,
		e_color_clear_value = 0x0C22,
		e_color_writemask = 0x0C23,
		e_doublebuffer = 0x0C32,
		e_stereo = 0x0C33,
		e_line_smooth_hint = 0x0C52,
		e_polygon_smooth_hint = 0x0C53,
		e_unpack_swap_bytes = 0x0CF0,
		e_unpack_lsb_first = 0x0CF1,
		e_unpack_row_length = 0x0CF2,
		e_unpack_skip_rows = 0x0CF3,
		e_unpack_skip_pixels = 0x0CF4,
		e_unpack_alignment = 0x0CF5,
		e_pack_swap_bytes = 0x0D00,
		e_pack_lsb_first = 0x0D01,
		e_pack_row_length = 0x0D02,
		e_pack_skip_rows = 0x0D03,
		e_pack_skip_pixels = 0x0D04,
		e_pack_alignment = 0x0D05,
		e_max_texture_size = 0x0D33,
		e_max_viewport_dims = 0x0D3A,
		e_subpixel_bits = 0x0D50,
		e_texture_1d = 0x0DE0,
		e_texture_2d = 0x0DE1,
		e_texture_width = 0x1000,
		e_texture_height = 0x1001,
		e_texture_border_color = 0x1004,
		e_dont_care = 0x1100,
		e_fastest = 0x1101,
		e_nicest = 0x1102,
		e_byte = 0x1400,
		e_unsigned_byte = 0x1401,
		e_short = 0x1402,
		e_unsigned_short = 0x1403,
		e_int = 0x1404,
		e_unsigned_int = 0x1405,
		e_float = 0x1406,
		e_stack_overflow = 0x0503,
		e_stack_underflow = 0x0504,
		e_clear = 0x1500,
		e_and = 0x1501,
		e_and_reverse = 0x1502,
		e_copy = 0x1503,
		e_and_inverted = 0x1504,
		e_noop = 0x1505,
		e_xor = 0x1506,
		e_or = 0x1507,
		e_nor = 0x1508,
		e_equiv = 0x1509,
		e_invert = 0x150A,
		e_or_reverse = 0x150B,
		e_copy_inverted = 0x150C,
		e_or_inverted = 0x150D,
		e_nand = 0x150E,
		e_set = 0x150F,
		e_texture = 0x1702,
		e_color = 0x1800,
		e_depth = 0x1801,
		e_stencil = 0x1802,
		e_stencil_index = 0x1901,
		e_depth_component = 0x1902,
		e_red = 0x1903,
		e_green = 0x1904,
		e_blue = 0x1905,
		e_alpha = 0x1906,
		e_rgb = 0x1907,
		e_rgba = 0x1908,
		e_point = 0x1B00,
		e_line = 0x1B01,
		e_fill = 0x1B02,
		e_keep = 0x1E00,
		e_replace = 0x1E01,
		e_incr = 0x1E02,
		e_decr = 0x1E03,
		e_vendor = 0x1F00,
		e_renderer = 0x1F01,
		e_version = 0x1F02,
		e_extensions = 0x1F03,
		e_nearest = 0x2600,
		e_linear = 0x2601,
		e_nearest_mipmap_nearest = 0x2700,
		e_linear_mipmap_nearest = 0x2701,
		e_nearest_mipmap_linear = 0x2702,
		e_linear_mipmap_linear = 0x2703,
		e_texture_mag_filter = 0x2800,
		e_texture_min_filter = 0x2801,
		e_texture_wrap_s = 0x2802,
		e_texture_wrap_t = 0x2803,
		e_repeat = 0x2901,
		e_color_logic_op = 0x0BF2,
		e_polygon_offset_units = 0x2A00,
		e_polygon_offset_point = 0x2A01,
		e_polygon_offset_line = 0x2A02,
		e_polygon_offset_fill = 0x8037,
		e_polygon_offset_factor = 0x8038,
		e_texture_binding_1d = 0x8068,
		e_texture_binding_2d = 0x8069,
		e_texture_internal_format = 0x1003,
		e_texture_red_size = 0x805C,
		e_texture_green_size = 0x805D,
		e_texture_blue_size = 0x805E,
		e_texture_alpha_size = 0x805F,
		e_double = 0x140A,
		e_proxy_texture_1d = 0x8063,
		e_proxy_texture_2d = 0x8064,
		e_r3_g3_b2 = 0x2A10,
		e_rgb4 = 0x804F,
		e_rgb5 = 0x8050,
		e_rgb8 = 0x8051,
		e_rgb10 = 0x8052,
		e_rgb12 = 0x8053,
		e_rgb16 = 0x8054,
		e_rgba2 = 0x8055,
		e_rgba4 = 0x8056,
		e_rgb5_a1 = 0x8057,
		e_rgba8 = 0x8058,
		e_rgb10_a2 = 0x8059,
		e_rgba12 = 0x805A,
		e_rgba16 = 0x805B,
		e_vertex_array = 0x8074,
		e_unsigned_byte_3_3_2 = 0x8032,
		e_unsigned_short_4_4_4_4 = 0x8033,
		e_unsigned_short_5_5_5_1 = 0x8034,
		e_unsigned_int_8_8_8_8 = 0x8035,
		e_unsigned_int_10_10_10_2 = 0x8036,
		e_texture_binding_3d = 0x806A,
		e_pack_skip_images = 0x806B,
		e_pack_image_height = 0x806C,
		e_unpack_skip_images = 0x806D,
		e_unpack_image_height = 0x806E,
		e_texture_3d = 0x806F,
		e_proxy_texture_3d = 0x8070,
		e_texture_depth = 0x8071,
		e_texture_wrap_r = 0x8072,
		e_max_3d_texture_size = 0x8073,
		e_unsigned_byte_2_3_3_rev = 0x8362,
		e_unsigned_short_5_6_5 = 0x8363,
		e_unsigned_short_5_6_5_rev = 0x8364,
		e_unsigned_short_4_4_4_4_rev = 0x8365,
		e_unsigned_short_1_5_5_5_rev = 0x8366,
		e_unsigned_int_8_8_8_8_rev = 0x8367,
		e_unsigned_int_2_10_10_10_rev = 0x8368,
		e_bgr = 0x80E0,
		e_bgra = 0x80E1,
		e_max_elements_vertices = 0x80E8,
		e_max_elements_indices = 0x80E9,
		e_clamp_to_edge = 0x812F,
		e_texture_min_lod = 0x813A,
		e_texture_max_lod = 0x813B,
		e_texture_base_level = 0x813C,
		e_texture_max_level = 0x813D,
		e_smooth_point_size_range = 0x0B12,
		e_smooth_point_size_granularity = 0x0B13,
		e_smooth_line_width_range = 0x0B22,
		e_smooth_line_width_granularity = 0x0B23,
		e_aliased_line_width_range = 0x846E,
		e_texture0 = 0x84C0,
		e_texture1 = 0x84C1,
		e_texture2 = 0x84C2,
		e_texture3 = 0x84C3,
		e_texture4 = 0x84C4,
		e_texture5 = 0x84C5,
		e_texture6 = 0x84C6,
		e_texture7 = 0x84C7,
		e_texture8 = 0x84C8,
		e_texture9 = 0x84C9,
		e_texture10 = 0x84CA,
		e_texture11 = 0x84CB,
		e_texture12 = 0x84CC,
		e_texture13 = 0x84CD,
		e_texture14 = 0x84CE,
		e_texture15 = 0x84CF,
		e_texture16 = 0x84D0,
		e_texture17 = 0x84D1,
		e_texture18 = 0x84D2,
		e_texture19 = 0x84D3,
		e_texture20 = 0x84D4,
		e_texture21 = 0x84D5,
		e_texture22 = 0x84D6,
		e_texture23 = 0x84D7,
		e_texture24 = 0x84D8,
		e_texture25 = 0x84D9,
		e_texture26 = 0x84DA,
		e_texture27 = 0x84DB,
		e_texture28 = 0x84DC,
		e_texture29 = 0x84DD,
		e_texture30 = 0x84DE,
		e_texture31 = 0x84DF,
		e_active_texture = 0x84E0,
		e_multisample = 0x809D,
		e_sample_alpha_to_coverage = 0x809E,
		e_sample_alpha_to_one = 0x809F,
		e_sample_coverage = 0x80A0,
		e_sample_buffers = 0x80A8,
		e_samples = 0x80A9,
		e_sample_coverage_value = 0x80AA,
		e_sample_coverage_invert = 0x80AB,
		e_texture_cube_map = 0x8513,
		e_texture_binding_cube_map = 0x8514,
		e_texture_cube_map_positive_x = 0x8515,
		e_texture_cube_map_negative_x = 0x8516,
		e_texture_cube_map_positive_y = 0x8517,
		e_texture_cube_map_negative_y = 0x8518,
		e_texture_cube_map_positive_z = 0x8519,
		e_texture_cube_map_negative_z = 0x851A,
		e_proxy_texture_cube_map = 0x851B,
		e_max_cube_map_texture_size = 0x851C,
		e_compressed_rgb = 0x84ED,
		e_compressed_rgba = 0x84EE,
		e_texture_compression_hint = 0x84EF,
		e_texture_compressed_image_size = 0x86A0,
		e_texture_compressed = 0x86A1,
		e_num_compressed_texture_formats = 0x86A2,
		e_compressed_texture_formats = 0x86A3,
		e_clamp_to_border = 0x812D,
		e_blend_dst_rgb = 0x80C8,
		e_blend_src_rgb = 0x80C9,
		e_blend_dst_alpha = 0x80CA,
		e_blend_src_alpha = 0x80CB,
		e_point_fade_threshold_size = 0x8128,
		e_depth_component16 = 0x81A5,
		e_depth_component24 = 0x81A6,
		e_depth_component32 = 0x81A7,
		e_mirrored_repeat = 0x8370,
		e_max_texture_lod_bias = 0x84FD,
		e_texture_lod_bias = 0x8501,
		e_incr_wrap = 0x8507,
		e_decr_wrap = 0x8508,
		e_texture_depth_size = 0x884A,
		e_texture_compare_mode = 0x884C,
		e_texture_compare_func = 0x884D,
		e_blend_color = 0x8005,
		e_blend_equation = 0x8009,
		e_constant_color = 0x8001,
		e_one_minus_constant_color = 0x8002,
		e_constant_alpha = 0x8003,
		e_one_minus_constant_alpha = 0x8004,
		e_func_add = 0x8006,
		e_func_reverse_subtract = 0x800B,
		e_func_subtract = 0x800A,
		e_min = 0x8007,
		e_max = 0x8008,
		e_buffer_size = 0x8764,
		e_buffer_usage = 0x8765,
		e_query_counter_bits = 0x8864,
		e_current_query = 0x8865,
		e_query_result = 0x8866,
		e_query_result_available = 0x8867,
		e_array_buffer = 0x8892,
		e_element_array_buffer = 0x8893,
		e_array_buffer_binding = 0x8894,
		e_element_array_buffer_binding = 0x8895,
		e_vertex_attrib_array_buffer_binding = 0x889F,
		e_read_only = 0x88B8,
		e_write_only = 0x88B9,
		e_read_write = 0x88BA,
		e_buffer_access = 0x88BB,
		e_buffer_mapped = 0x88BC,
		e_buffer_map_pointer = 0x88BD,
		e_stream_draw = 0x88E0,
		e_stream_read = 0x88E1,
		e_stream_copy = 0x88E2,
		e_static_draw = 0x88E4,
		e_static_read = 0x88E5,
		e_static_copy = 0x88E6,
		e_dynamic_draw = 0x88E8,
		e_dynamic_read = 0x88E9,
		e_dynamic_copy = 0x88EA,
		e_samples_passed = 0x8914,
		e_src1_alpha = 0x8589,
		e_blend_equation_rgb = 0x8009,
		e_vertex_attrib_array_enabled = 0x8622,
		e_vertex_attrib_array_size = 0x8623,
		e_vertex_attrib_array_stride = 0x8624,
		e_vertex_attrib_array_type = 0x8625,
		e_current_vertex_attrib = 0x8626,
		e_vertex_program_point_size = 0x8642,
		e_vertex_attrib_array_pointer = 0x8645,
		e_stencil_back_func = 0x8800,
		e_stencil_back_fail = 0x8801,
		e_stencil_back_pass_depth_fail = 0x8802,
		e_stencil_back_pass_depth_pass = 0x8803,
		e_max_draw_buffers = 0x8824,
		e_draw_buffer0 = 0x8825,
		e_draw_buffer1 = 0x8826,
		e_draw_buffer2 = 0x8827,
		e_draw_buffer3 = 0x8828,
		e_draw_buffer4 = 0x8829,
		e_draw_buffer5 = 0x882A,
		e_draw_buffer6 = 0x882B,
		e_draw_buffer7 = 0x882C,
		e_draw_buffer8 = 0x882D,
		e_draw_buffer9 = 0x882E,
		e_draw_buffer10 = 0x882F,
		e_draw_buffer11 = 0x8830,
		e_draw_buffer12 = 0x8831,
		e_draw_buffer13 = 0x8832,
		e_draw_buffer14 = 0x8833,
		e_draw_buffer15 = 0x8834,
		e_blend_equation_alpha = 0x883D,
		e_max_vertex_attribs = 0x8869,
		e_vertex_attrib_array_normalized = 0x886A,
		e_max_texture_image_units = 0x8872,
		e_fragment_shader = 0x8B30,
		e_vertex_shader = 0x8B31,
		e_max_fragment_uniform_components = 0x8B49,
		e_max_vertex_uniform_components = 0x8B4A,
		e_max_varying_floats = 0x8B4B,
		e_max_vertex_texture_image_units = 0x8B4C,
		e_max_combined_texture_image_units = 0x8B4D,
		e_shader_type = 0x8B4F,
		e_float_vec2 = 0x8B50,
		e_float_vec3 = 0x8B51,
		e_float_vec4 = 0x8B52,
		e_int_vec2 = 0x8B53,
		e_int_vec3 = 0x8B54,
		e_int_vec4 = 0x8B55,
		e_bool = 0x8B56,
		e_bool_vec2 = 0x8B57,
		e_bool_vec3 = 0x8B58,
		e_bool_vec4 = 0x8B59,
		e_float_mat2 = 0x8B5A,
		e_float_mat3 = 0x8B5B,
		e_float_mat4 = 0x8B5C,
		e_sampler_1d = 0x8B5D,
		e_sampler_2d = 0x8B5E,
		e_sampler_3d = 0x8B5F,
		e_sampler_cube = 0x8B60,
		e_sampler_1d_shadow = 0x8B61,
		e_sampler_2d_shadow = 0x8B62,
		e_delete_status = 0x8B80,
		e_compile_status = 0x8B81,
		e_link_status = 0x8B82,
		e_validate_status = 0x8B83,
		e_info_log_length = 0x8B84,
		e_attached_shaders = 0x8B85,
		e_active_uniforms = 0x8B86,
		e_active_uniform_max_length = 0x8B87,
		e_shader_source_length = 0x8B88,
		e_active_attributes = 0x8B89,
		e_active_attribute_max_length = 0x8B8A,
		e_fragment_shader_derivative_hint = 0x8B8B,
		e_shading_language_version = 0x8B8C,
		e_current_program = 0x8B8D,
		e_point_sprite_coord_origin = 0x8CA0,
		e_lower_left = 0x8CA1,
		e_upper_left = 0x8CA2,
		e_stencil_back_ref = 0x8CA3,
		e_stencil_back_value_mask = 0x8CA4,
		e_stencil_back_writemask = 0x8CA5,
		e_pixel_pack_buffer = 0x88EB,
		e_pixel_unpack_buffer = 0x88EC,
		e_pixel_pack_buffer_binding = 0x88ED,
		e_pixel_unpack_buffer_binding = 0x88EF,
		e_float_mat2x3 = 0x8B65,
		e_float_mat2x4 = 0x8B66,
		e_float_mat3x2 = 0x8B67,
		e_float_mat3x4 = 0x8B68,
		e_float_mat4x2 = 0x8B69,
		e_float_mat4x3 = 0x8B6A,
		e_srgb = 0x8C40,
		e_srgb8 = 0x8C41,
		e_srgb_alpha = 0x8C42,
		e_srgb8_alpha8 = 0x8C43,
		e_compressed_srgb = 0x8C48,
		e_compressed_srgb_alpha = 0x8C49,
		e_compare_ref_to_texture = 0x884E,
		e_clip_distance0 = 0x3000,
		e_clip_distance1 = 0x3001,
		e_clip_distance2 = 0x3002,
		e_clip_distance3 = 0x3003,
		e_clip_distance4 = 0x3004,
		e_clip_distance5 = 0x3005,
		e_clip_distance6 = 0x3006,
		e_clip_distance7 = 0x3007,
		e_max_clip_distances = 0x0D32,
		e_major_version = 0x821B,
		e_minor_version = 0x821C,
		e_num_extensions = 0x821D,
		e_context_flags = 0x821E,
		e_compressed_red = 0x8225,
		e_compressed_rg = 0x8226,
		e_context_flag_forward_compatible_bit = 0x00000001,
		e_rgba32f = 0x8814,
		e_rgb32f = 0x8815,
		e_rgba16f = 0x881A,
		e_rgb16f = 0x881B,
		e_vertex_attrib_array_integer = 0x88FD,
		e_max_array_texture_layers = 0x88FF,
		e_min_program_texel_offset = 0x8904,
		e_max_program_texel_offset = 0x8905,
		e_clamp_read_color = 0x891C,
		e_fixed_only = 0x891D,
		e_max_varying_components = 0x8B4B,
		e_texture_1d_array = 0x8C18,
		e_proxy_texture_1d_array = 0x8C19,
		e_texture_2d_array = 0x8C1A,
		e_proxy_texture_2d_array = 0x8C1B,
		e_texture_binding_1d_array = 0x8C1C,
		e_texture_binding_2d_array = 0x8C1D,
		e_r11f_g11f_b10f = 0x8C3A,
		e_unsigned_int_10f_11f_11f_rev = 0x8C3B,
		e_rgb9_e5 = 0x8C3D,
		e_unsigned_int_5_9_9_9_rev = 0x8C3E,
		e_texture_shared_size = 0x8C3F,
		e_transform_feedback_varying_max_length = 0x8C76,
		e_transform_feedback_buffer_mode = 0x8C7F,
		e_max_transform_feedback_separate_components = 0x8C80,
		e_transform_feedback_varyings = 0x8C83,
		e_transform_feedback_buffer_start = 0x8C84,
		e_transform_feedback_buffer_size = 0x8C85,
		e_primitives_generated = 0x8C87,
		e_transform_feedback_primitives_written = 0x8C88,
		e_rasterizer_discard = 0x8C89,
		e_max_transform_feedback_interleaved_components = 0x8C8A,
		e_max_transform_feedback_separate_attribs = 0x8C8B,
		e_interleaved_attribs = 0x8C8C,
		e_separate_attribs = 0x8C8D,
		e_transform_feedback_buffer = 0x8C8E,
		e_transform_feedback_buffer_binding = 0x8C8F,
		e_rgba32ui = 0x8D70,
		e_rgb32ui = 0x8D71,
		e_rgba16ui = 0x8D76,
		e_rgb16ui = 0x8D77,
		e_rgba8ui = 0x8D7C,
		e_rgb8ui = 0x8D7D,
		e_rgba32i = 0x8D82,
		e_rgb32i = 0x8D83,
		e_rgba16i = 0x8D88,
		e_rgb16i = 0x8D89,
		e_rgba8i = 0x8D8E,
		e_rgb8i = 0x8D8F,
		e_red_integer = 0x8D94,
		e_green_integer = 0x8D95,
		e_blue_integer = 0x8D96,
		e_rgb_integer = 0x8D98,
		e_rgba_integer = 0x8D99,
		e_bgr_integer = 0x8D9A,
		e_bgra_integer = 0x8D9B,
		e_sampler_1d_array = 0x8DC0,
		e_sampler_2d_array = 0x8DC1,
		e_sampler_1d_array_shadow = 0x8DC3,
		e_sampler_2d_array_shadow = 0x8DC4,
		e_sampler_cube_shadow = 0x8DC5,
		e_unsigned_int_vec2 = 0x8DC6,
		e_unsigned_int_vec3 = 0x8DC7,
		e_unsigned_int_vec4 = 0x8DC8,
		e_int_sampler_1d = 0x8DC9,
		e_int_sampler_2d = 0x8DCA,
		e_int_sampler_3d = 0x8DCB,
		e_int_sampler_cube = 0x8DCC,
		e_int_sampler_1d_array = 0x8DCE,
		e_int_sampler_2d_array = 0x8DCF,
		e_unsigned_int_sampler_1d = 0x8DD1,
		e_unsigned_int_sampler_2d = 0x8DD2,
		e_unsigned_int_sampler_3d = 0x8DD3,
		e_unsigned_int_sampler_cube = 0x8DD4,
		e_unsigned_int_sampler_1d_array = 0x8DD6,
		e_unsigned_int_sampler_2d_array = 0x8DD7,
		e_query_wait = 0x8E13,
		e_query_no_wait = 0x8E14,
		e_query_by_region_wait = 0x8E15,
		e_query_by_region_no_wait = 0x8E16,
		e_buffer_access_flags = 0x911F,
		e_buffer_map_length = 0x9120,
		e_buffer_map_offset = 0x9121,
		e_depth_component32f = 0x8CAC,
		e_depth32f_stencil8 = 0x8CAD,
		e_float_32_unsigned_int_24_8_rev = 0x8DAD,
		e_invalid_framebuffer_operation = 0x0506,
		e_framebuffer_attachment_color_encoding = 0x8210,
		e_framebuffer_attachment_component_type = 0x8211,
		e_framebuffer_attachment_red_size = 0x8212,
		e_framebuffer_attachment_green_size = 0x8213,
		e_framebuffer_attachment_blue_size = 0x8214,
		e_framebuffer_attachment_alpha_size = 0x8215,
		e_framebuffer_attachment_depth_size = 0x8216,
		e_framebuffer_attachment_stencil_size = 0x8217,
		e_framebuffer_default = 0x8218,
		e_framebuffer_undefined = 0x8219,
		e_depth_stencil_attachment = 0x821A,
		e_max_renderbuffer_size = 0x84E8,
		e_depth_stencil = 0x84F9,
		e_unsigned_int_24_8 = 0x84FA,
		e_depth24_stencil8 = 0x88F0,
		e_texture_stencil_size = 0x88F1,
		e_texture_red_type = 0x8C10,
		e_texture_green_type = 0x8C11,
		e_texture_blue_type = 0x8C12,
		e_texture_alpha_type = 0x8C13,
		e_texture_depth_type = 0x8C16,
		e_unsigned_normalized = 0x8C17,
		e_framebuffer_binding = 0x8CA6,
		e_draw_framebuffer_binding = 0x8CA6,
		e_renderbuffer_binding = 0x8CA7,
		e_read_framebuffer = 0x8CA8,
		e_draw_framebuffer = 0x8CA9,
		e_read_framebuffer_binding = 0x8CAA,
		e_renderbuffer_samples = 0x8CAB,
		e_framebuffer_attachment_object_type = 0x8CD0,
		e_framebuffer_attachment_object_name = 0x8CD1,
		e_framebuffer_attachment_texture_level = 0x8CD2,
		e_framebuffer_attachment_texture_cube_map_face = 0x8CD3,
		e_framebuffer_attachment_texture_layer = 0x8CD4,
		e_framebuffer_complete = 0x8CD5,
		e_framebuffer_incomplete_attachment = 0x8CD6,
		e_framebuffer_incomplete_missing_attachment = 0x8CD7,
		e_framebuffer_incomplete_draw_buffer = 0x8CDB,
		e_framebuffer_incomplete_read_buffer = 0x8CDC,
		e_framebuffer_unsupported = 0x8CDD,
		e_max_color_attachments = 0x8CDF,
		e_color_attachment0 = 0x8CE0,
		e_color_attachment1 = 0x8CE1,
		e_color_attachment2 = 0x8CE2,
		e_color_attachment3 = 0x8CE3,
		e_color_attachment4 = 0x8CE4,
		e_color_attachment5 = 0x8CE5,
		e_color_attachment6 = 0x8CE6,
		e_color_attachment7 = 0x8CE7,
		e_color_attachment8 = 0x8CE8,
		e_color_attachment9 = 0x8CE9,
		e_color_attachment10 = 0x8CEA,
		e_color_attachment11 = 0x8CEB,
		e_color_attachment12 = 0x8CEC,
		e_color_attachment13 = 0x8CED,
		e_color_attachment14 = 0x8CEE,
		e_color_attachment15 = 0x8CEF,
		e_color_attachment16 = 0x8CF0,
		e_color_attachment17 = 0x8CF1,
		e_color_attachment18 = 0x8CF2,
		e_color_attachment19 = 0x8CF3,
		e_color_attachment20 = 0x8CF4,
		e_color_attachment21 = 0x8CF5,
		e_color_attachment22 = 0x8CF6,
		e_color_attachment23 = 0x8CF7,
		e_color_attachment24 = 0x8CF8,
		e_color_attachment25 = 0x8CF9,
		e_color_attachment26 = 0x8CFA,
		e_color_attachment27 = 0x8CFB,
		e_color_attachment28 = 0x8CFC,
		e_color_attachment29 = 0x8CFD,
		e_color_attachment30 = 0x8CFE,
		e_color_attachment31 = 0x8CFF,
		e_depth_attachment = 0x8D00,
		e_stencil_attachment = 0x8D20,
		e_framebuffer = 0x8D40,
		e_renderbuffer = 0x8D41,
		e_renderbuffer_width = 0x8D42,
		e_renderbuffer_height = 0x8D43,
		e_renderbuffer_internal_format = 0x8D44,
		e_stencil_index1 = 0x8D46,
		e_stencil_index4 = 0x8D47,
		e_stencil_index8 = 0x8D48,
		e_stencil_index16 = 0x8D49,
		e_renderbuffer_red_size = 0x8D50,
		e_renderbuffer_green_size = 0x8D51,
		e_renderbuffer_blue_size = 0x8D52,
		e_renderbuffer_alpha_size = 0x8D53,
		e_renderbuffer_depth_size = 0x8D54,
		e_renderbuffer_stencil_size = 0x8D55,
		e_framebuffer_incomplete_multisample = 0x8D56,
		e_max_samples = 0x8D57,
		e_framebuffer_srgb = 0x8DB9,
		e_half_float = 0x140B,
		e_map_read_bit = 0x0001,
		e_map_write_bit = 0x0002,
		e_map_invalidate_range_bit = 0x0004,
		e_map_invalidate_buffer_bit = 0x0008,
		e_map_flush_explicit_bit = 0x0010,
		e_map_unsynchronized_bit = 0x0020,
		e_compressed_red_rgtc1 = 0x8DBB,
		e_compressed_signed_red_rgtc1 = 0x8DBC,
		e_compressed_rg_rgtc2 = 0x8DBD,
		e_compressed_signed_rg_rgtc2 = 0x8DBE,
		e_rg = 0x8227,
		e_rg_integer = 0x8228,
		e_r8 = 0x8229,
		e_r16 = 0x822A,
		e_rg8 = 0x822B,
		e_rg16 = 0x822C,
		e_r16f = 0x822D,
		e_r32f = 0x822E,
		e_rg16f = 0x822F,
		e_rg32f = 0x8230,
		e_r8i = 0x8231,
		e_r8ui = 0x8232,
		e_r16i = 0x8233,
		e_r16ui = 0x8234,
		e_r32i = 0x8235,
		e_r32ui = 0x8236,
		e_rg8i = 0x8237,
		e_rg8ui = 0x8238,
		e_rg16i = 0x8239,
		e_rg16ui = 0x823A,
		e_rg32i = 0x823B,
		e_rg32ui = 0x823C,
		e_vertex_array_binding = 0x85B5,
		e_sampler_2d_rect = 0x8B63,
		e_sampler_2d_rect_shadow = 0x8B64,
		e_sampler_buffer = 0x8DC2,
		e_int_sampler_2d_rect = 0x8DCD,
		e_int_sampler_buffer = 0x8DD0,
		e_unsigned_int_sampler_2d_rect = 0x8DD5,
		e_unsigned_int_sampler_buffer = 0x8DD8,
		e_texture_buffer = 0x8C2A,
		e_max_texture_buffer_size = 0x8C2B,
		e_texture_binding_buffer = 0x8C2C,
		e_texture_buffer_data_store_binding = 0x8C2D,
		e_texture_rectangle = 0x84F5,
		e_texture_binding_rectangle = 0x84F6,
		e_proxy_texture_rectangle = 0x84F7,
		e_max_rectangle_texture_size = 0x84F8,
		e_r8_snorm = 0x8F94,
		e_rg8_snorm = 0x8F95,
		e_rgb8_snorm = 0x8F96,
		e_rgba8_snorm = 0x8F97,
		e_r16_snorm = 0x8F98,
		e_rg16_snorm = 0x8F99,
		e_rgb16_snorm = 0x8F9A,
		e_rgba16_snorm = 0x8F9B,
		e_signed_normalized = 0x8F9C,
		e_primitive_restart = 0x8F9D,
		e_primitive_restart_index = 0x8F9E,
		e_copy_read_buffer = 0x8F36,
		e_copy_write_buffer = 0x8F37,
		e_uniform_buffer = 0x8A11,
		e_uniform_buffer_binding = 0x8A28,
		e_uniform_buffer_start = 0x8A29,
		e_uniform_buffer_size = 0x8A2A,
		e_max_vertex_uniform_blocks = 0x8A2B,
		e_max_geometry_uniform_blocks = 0x8A2C,
		e_max_fragment_uniform_blocks = 0x8A2D,
		e_max_combined_uniform_blocks = 0x8A2E,
		e_max_uniform_buffer_bindings = 0x8A2F,
		e_max_uniform_block_size = 0x8A30,
		e_max_combined_vertex_uniform_components = 0x8A31,
		e_max_combined_geometry_uniform_components = 0x8A32,
		e_max_combined_fragment_uniform_components = 0x8A33,
		e_uniform_buffer_offset_alignment = 0x8A34,
		e_active_uniform_block_max_name_length = 0x8A35,
		e_active_uniform_blocks = 0x8A36,
		e_uniform_type = 0x8A37,
		e_uniform_size = 0x8A38,
		e_uniform_name_length = 0x8A39,
		e_uniform_block_index = 0x8A3A,
		e_uniform_offset = 0x8A3B,
		e_uniform_array_stride = 0x8A3C,
		e_uniform_matrix_stride = 0x8A3D,
		e_uniform_is_row_major = 0x8A3E,
		e_uniform_block_binding = 0x8A3F,
		e_uniform_block_data_size = 0x8A40,
		e_uniform_block_name_length = 0x8A41,
		e_uniform_block_active_uniforms = 0x8A42,
		e_uniform_block_active_uniform_indices = 0x8A43,
		e_uniform_block_referenced_by_vertex_shader = 0x8A44,
		e_uniform_block_referenced_by_geometry_shader = 0x8A45,
		e_uniform_block_referenced_by_fragment_shader = 0x8A46,
		e_invalid_index = 0xFFFFFFFFu,
		e_context_core_profile_bit = 0x00000001,
		e_context_compatibility_profile_bit = 0x00000002,
		e_lines_adjacency = 0x000A,
		e_line_strip_adjacency = 0x000B,
		e_triangles_adjacency = 0x000C,
		e_triangle_strip_adjacency = 0x000D,
		e_program_point_size = 0x8642,
		e_max_geometry_texture_image_units = 0x8C29,
		e_framebuffer_attachment_layered = 0x8DA7,
		e_framebuffer_incomplete_layer_targets = 0x8DA8,
		e_geometry_shader = 0x8DD9,
		e_geometry_vertices_out = 0x8916,
		e_geometry_input_type = 0x8917,
		e_geometry_output_type = 0x8918,
		e_max_geometry_uniform_components = 0x8DDF,
		e_max_geometry_output_vertices = 0x8DE0,
		e_max_geometry_total_output_components = 0x8DE1,
		e_max_vertex_output_components = 0x9122,
		e_max_geometry_input_components = 0x9123,
		e_max_geometry_output_components = 0x9124,
		e_max_fragment_input_components = 0x9125,
		e_context_profile_mask = 0x9126,
		e_depth_clamp = 0x864F,
		e_quads_follow_provoking_vertex_convention = 0x8E4C,
		e_first_vertex_convention = 0x8E4D,
		e_last_vertex_convention = 0x8E4E,
		e_provoking_vertex = 0x8E4F,
		e_texture_cube_map_seamless = 0x884F,
		e_max_server_wait_timeout = 0x9111,
		e_object_type = 0x9112,
		e_sync_condition = 0x9113,
		e_sync_status = 0x9114,
		e_sync_flags = 0x9115,
		e_sync_fence = 0x9116,
		e_sync_gpu_commands_complete = 0x9117,
		e_unsignaled = 0x9118,
		e_signaled = 0x9119,
		e_already_signaled = 0x911A,
		e_timeout_expired = 0x911B,
		e_condition_satisfied = 0x911C,
		e_wait_failed = 0x911D,
		e_timeout_ignored = 0xFFFFFFFFFFFFFFFFull,
		e_sync_flush_commands_bit = 0x00000001,
		e_sample_position = 0x8E50,
		e_sample_mask = 0x8E51,
		e_sample_mask_value = 0x8E52,
		e_max_sample_mask_words = 0x8E59,
		e_texture_2d_multisample = 0x9100,
		e_proxy_texture_2d_multisample = 0x9101,
		e_texture_2d_multisample_array = 0x9102,
		e_proxy_texture_2d_multisample_array = 0x9103,
		e_texture_binding_2d_multisample = 0x9104,
		e_texture_binding_2d_multisample_array = 0x9105,
		e_texture_samples = 0x9106,
		e_texture_fixed_sample_locations = 0x9107,
		e_sampler_2d_multisample = 0x9108,
		e_int_sampler_2d_multisample = 0x9109,
		e_unsigned_int_sampler_2d_multisample = 0x910A,
		e_sampler_2d_multisample_array = 0x910B,
		e_int_sampler_2d_multisample_array = 0x910C,
		e_unsigned_int_sampler_2d_multisample_array = 0x910D,
		e_max_color_texture_samples = 0x910E,
		e_max_depth_texture_samples = 0x910F,
		e_max_integer_samples = 0x9110,
		e_vertex_attrib_array_divisor = 0x88FE,
		e_src1_color = 0x88F9,
		e_one_minus_src1_color = 0x88FA,
		e_one_minus_src1_alpha = 0x88FB,
		e_max_dual_source_draw_buffers = 0x88FC,
		e_any_samples_passed = 0x8C2F,
		e_sampler_binding = 0x8919,
		e_rgb10_a2ui = 0x906F,
		e_texture_swizzle_r = 0x8E42,
		e_texture_swizzle_g = 0x8E43,
		e_texture_swizzle_b = 0x8E44,
		e_texture_swizzle_a = 0x8E45,
		e_texture_swizzle_rgba = 0x8E46,
		e_time_elapsed = 0x88BF,
		e_timestamp = 0x8E28,
		e_int_2_10_10_10_rev = 0x8D9F,
		e_sample_shading = 0x8C36,
		e_min_sample_shading_value = 0x8C37,
		e_min_program_texture_gather_offset = 0x8E5E,
		e_max_program_texture_gather_offset = 0x8E5F,
		e_texture_cube_map_array = 0x9009,
		e_texture_binding_cube_map_array = 0x900A,
		e_proxy_texture_cube_map_array = 0x900B,
		e_sampler_cube_map_array = 0x900C,
		e_sampler_cube_map_array_shadow = 0x900D,
		e_int_sampler_cube_map_array = 0x900E,
		e_unsigned_int_sampler_cube_map_array = 0x900F,
		e_draw_indirect_buffer = 0x8F3F,
		e_draw_indirect_buffer_binding = 0x8F43,
		e_geometry_shader_invocations = 0x887F,
		e_max_geometry_shader_invocations = 0x8E5A,
		e_min_fragment_interpolation_offset = 0x8E5B,
		e_max_fragment_interpolation_offset = 0x8E5C,
		e_fragment_interpolation_offset_bits = 0x8E5D,
		e_max_vertex_streams = 0x8E71,
		e_double_vec2 = 0x8FFC,
		e_double_vec3 = 0x8FFD,
		e_double_vec4 = 0x8FFE,
		e_double_mat2 = 0x8F46,
		e_double_mat3 = 0x8F47,
		e_double_mat4 = 0x8F48,
		e_double_mat2x3 = 0x8F49,
		e_double_mat2x4 = 0x8F4A,
		e_double_mat3x2 = 0x8F4B,
		e_double_mat3x4 = 0x8F4C,
		e_double_mat4x2 = 0x8F4D,
		e_double_mat4x3 = 0x8F4E,
		e_active_subroutines = 0x8DE5,
		e_active_subroutine_uniforms = 0x8DE6,
		e_active_subroutine_uniform_locations = 0x8E47,
		e_active_subroutine_max_length = 0x8E48,
		e_active_subroutine_uniform_max_length = 0x8E49,
		e_max_subroutines = 0x8DE7,
		e_max_subroutine_uniform_locations = 0x8DE8,
		e_num_compatible_subroutines = 0x8E4A,
		e_compatible_subroutines = 0x8E4B,
		e_patches = 0x000E,
		e_patch_vertices = 0x8E72,
		e_patch_default_inner_level = 0x8E73,
		e_patch_default_outer_level = 0x8E74,
		e_tess_control_output_vertices = 0x8E75,
		e_tess_gen_mode = 0x8E76,
		e_tess_gen_spacing = 0x8E77,
		e_tess_gen_vertex_order = 0x8E78,
		e_tess_gen_point_mode = 0x8E79,
		e_isolines = 0x8E7A,
		e_fractional_odd = 0x8E7B,
		e_fractional_even = 0x8E7C,
		e_max_patch_vertices = 0x8E7D,
		e_max_tess_gen_level = 0x8E7E,
		e_max_tess_control_uniform_components = 0x8E7F,
		e_max_tess_evaluation_uniform_components = 0x8E80,
		e_max_tess_control_texture_image_units = 0x8E81,
		e_max_tess_evaluation_texture_image_units = 0x8E82,
		e_max_tess_control_output_components = 0x8E83,
		e_max_tess_patch_components = 0x8E84,
		e_max_tess_control_total_output_components = 0x8E85,
		e_max_tess_evaluation_output_components = 0x8E86,
		e_max_tess_control_uniform_blocks = 0x8E89,
		e_max_tess_evaluation_uniform_blocks = 0x8E8A,
		e_max_tess_control_input_components = 0x886C,
		e_max_tess_evaluation_input_components = 0x886D,
		e_max_combined_tess_control_uniform_components = 0x8E1E,
		e_max_combined_tess_evaluation_uniform_components = 0x8E1F,
		e_uniform_block_referenced_by_tess_control_shader = 0x84F0,
		e_uniform_block_referenced_by_tess_evaluation_shader = 0x84F1,
		e_tess_evaluation_shader = 0x8E87,
		e_tess_control_shader = 0x8E88,
		e_transform_feedback = 0x8E22,
		e_transform_feedback_buffer_paused = 0x8E23,
		e_transform_feedback_buffer_active = 0x8E24,
		e_transform_feedback_binding = 0x8E25,
		e_max_transform_feedback_buffers = 0x8E70,
		e_fixed = 0x140C,
		e_implementation_color_read_type = 0x8B9A,
		e_implementation_color_read_format = 0x8B9B,
		e_low_float = 0x8DF0,
		e_medium_float = 0x8DF1,
		e_high_float = 0x8DF2,
		e_low_int = 0x8DF3,
		e_medium_int = 0x8DF4,
		e_high_int = 0x8DF5,
		e_shader_compiler = 0x8DFA,
		e_shader_binary_formats = 0x8DF8,
		e_num_shader_binary_formats = 0x8DF9,
		e_max_vertex_uniform_vectors = 0x8DFB,
		e_max_varying_vectors = 0x8DFC,
		e_max_fragment_uniform_vectors = 0x8DFD,
		e_rgb565 = 0x8D62,
		e_program_binary_retrievable_hint = 0x8257,
		e_program_binary_length = 0x8741,
		e_num_program_binary_formats = 0x87FE,
		e_program_binary_formats = 0x87FF,
		e_vertex_shader_bit = 0x00000001,
		e_fragment_shader_bit = 0x00000002,
		e_geometry_shader_bit = 0x00000004,
		e_tess_control_shader_bit = 0x00000008,
		e_tess_evaluation_shader_bit = 0x00000010,
		e_all_shader_bits = 0xFFFFFFFF,
		e_program_separable = 0x8258,
		e_active_program = 0x8259,
		e_program_pipeline_binding = 0x825A,
		e_max_viewports = 0x825B,
		e_viewport_subpixel_bits = 0x825C,
		e_viewport_bounds_range = 0x825D,
		e_layer_provoking_vertex = 0x825E,
		e_viewport_index_provoking_vertex = 0x825F,
		e_undefined_vertex = 0x8260,
		e_copy_read_buffer_binding = 0x8F36,
		e_copy_write_buffer_binding = 0x8F37,
		e_transform_feedback_active = 0x8E24,
		e_transform_feedback_paused = 0x8E23,
		e_unpack_compressed_block_width = 0x9127,
		e_unpack_compressed_block_height = 0x9128,
		e_unpack_compressed_block_depth = 0x9129,
		e_unpack_compressed_block_size = 0x912A,
		e_pack_compressed_block_width = 0x912B,
		e_pack_compressed_block_height = 0x912C,
		e_pack_compressed_block_depth = 0x912D,
		e_pack_compressed_block_size = 0x912E,
		e_num_sample_counts = 0x9380,
		e_min_map_buffer_alignment = 0x90BC,
		e_atomic_counter_buffer = 0x92C0,
		e_atomic_counter_buffer_binding = 0x92C1,
		e_atomic_counter_buffer_start = 0x92C2,
		e_atomic_counter_buffer_size = 0x92C3,
		e_atomic_counter_buffer_data_size = 0x92C4,
		e_atomic_counter_buffer_active_atomic_counters = 0x92C5,
		e_atomic_counter_buffer_active_atomic_counter_indices = 0x92C6,
		e_atomic_counter_buffer_referenced_by_vertex_shader = 0x92C7,
		e_atomic_counter_buffer_referenced_by_tess_control_shader = 0x92C8,
		e_atomic_counter_buffer_referenced_by_tess_evaluation_shader = 0x92C9,
		e_atomic_counter_buffer_referenced_by_geometry_shader = 0x92CA,
		e_atomic_counter_buffer_referenced_by_fragment_shader = 0x92CB,
		e_max_vertex_atomic_counter_buffers = 0x92CC,
		e_max_tess_control_atomic_counter_buffers = 0x92CD,
		e_max_tess_evaluation_atomic_counter_buffers = 0x92CE,
		e_max_geometry_atomic_counter_buffers = 0x92CF,
		e_max_fragment_atomic_counter_buffers = 0x92D0,
		e_max_combined_atomic_counter_buffers = 0x92D1,
		e_max_vertex_atomic_counters = 0x92D2,
		e_max_tess_control_atomic_counters = 0x92D3,
		e_max_tess_evaluation_atomic_counters = 0x92D4,
		e_max_geometry_atomic_counters = 0x92D5,
		e_max_fragment_atomic_counters = 0x92D6,
		e_max_combined_atomic_counters = 0x92D7,
		e_max_atomic_counter_buffer_size = 0x92D8,
		e_max_atomic_counter_buffer_bindings = 0x92DC,
		e_active_atomic_counter_buffers = 0x92D9,
		e_uniform_atomic_counter_buffer_index = 0x92DA,
		e_unsigned_int_atomic_counter = 0x92DB,
		e_vertex_attrib_array_barrier_bit = 0x00000001,
		e_element_array_barrier_bit = 0x00000002,
		e_uniform_barrier_bit = 0x00000004,
		e_texture_fetch_barrier_bit = 0x00000008,
		e_shader_image_access_barrier_bit = 0x00000020,
		e_command_barrier_bit = 0x00000040,
		e_pixel_buffer_barrier_bit = 0x00000080,
		e_texture_update_barrier_bit = 0x00000100,
		e_buffer_update_barrier_bit = 0x00000200,
		e_framebuffer_barrier_bit = 0x00000400,
		e_transform_feedback_barrier_bit = 0x00000800,
		e_atomic_counter_barrier_bit = 0x00001000,
		e_all_barrier_bits = 0xFFFFFFFF,
		e_max_image_units = 0x8F38,
		e_max_combined_image_units_and_fragment_outputs = 0x8F39,
		e_image_binding_name = 0x8F3A,
		e_image_binding_level = 0x8F3B,
		e_image_binding_layered = 0x8F3C,
		e_image_binding_layer = 0x8F3D,
		e_image_binding_access = 0x8F3E,
		e_image_1d = 0x904C,
		e_image_2d = 0x904D,
		e_image_3d = 0x904E,
		e_image_2d_rect = 0x904F,
		e_image_cube = 0x9050,
		e_image_buffer = 0x9051,
		e_image_1d_array = 0x9052,
		e_image_2d_array = 0x9053,
		e_image_cube_map_array = 0x9054,
		e_image_2d_multisample = 0x9055,
		e_image_2d_multisample_array = 0x9056,
		e_int_image_1d = 0x9057,
		e_int_image_2d = 0x9058,
		e_int_image_3d = 0x9059,
		e_int_image_2d_rect = 0x905A,
		e_int_image_cube = 0x905B,
		e_int_image_buffer = 0x905C,
		e_int_image_1d_array = 0x905D,
		e_int_image_2d_array = 0x905E,
		e_int_image_cube_map_array = 0x905F,
		e_int_image_2d_multisample = 0x9060,
		e_int_image_2d_multisample_array = 0x9061,
		e_unsigned_int_image_1d = 0x9062,
		e_unsigned_int_image_2d = 0x9063,
		e_unsigned_int_image_3d = 0x9064,
		e_unsigned_int_image_2d_rect = 0x9065,
		e_unsigned_int_image_cube = 0x9066,
		e_unsigned_int_image_buffer = 0x9067,
		e_unsigned_int_image_1d_array = 0x9068,
		e_unsigned_int_image_2d_array = 0x9069,
		e_unsigned_int_image_cube_map_array = 0x906A,
		e_unsigned_int_image_2d_multisample = 0x906B,
		e_unsigned_int_image_2d_multisample_array = 0x906C,
		e_max_image_samples = 0x906D,
		e_image_binding_format = 0x906E,
		e_image_format_compatibility_type = 0x90C7,
		e_image_format_compatibility_by_size = 0x90C8,
		e_image_format_compatibility_by_class = 0x90C9,
		e_max_vertex_image_uniforms = 0x90CA,
		e_max_tess_control_image_uniforms = 0x90CB,
		e_max_tess_evaluation_image_uniforms = 0x90CC,
		e_max_geometry_image_uniforms = 0x90CD,
		e_max_fragment_image_uniforms = 0x90CE,
		e_max_combined_image_uniforms = 0x90CF,
		e_compressed_rgba_bptc_unorm = 0x8E8C,
		e_compressed_srgb_alpha_bptc_unorm = 0x8E8D,
		e_compressed_rgb_bptc_signed_float = 0x8E8E,
		e_compressed_rgb_bptc_unsigned_float = 0x8E8F,
		e_texture_immutable_format = 0x912F,
		e_num_shading_language_versions = 0x82E9,
		e_vertex_attrib_array_long = 0x874E,
		e_compressed_rgb8_etc2 = 0x9274,
		e_compressed_srgb8_etc2 = 0x9275,
		e_compressed_rgb8_punchthrough_alpha1_etc2 = 0x9276,
		e_compressed_srgb8_punchthrough_alpha1_etc2 = 0x9277,
		e_compressed_rgba8_etc2_eac = 0x9278,
		e_compressed_srgb8_alpha8_etc2_eac = 0x9279,
		e_compressed_r11_eac = 0x9270,
		e_compressed_signed_r11_eac = 0x9271,
		e_compressed_rg11_eac = 0x9272,
		e_compressed_signed_rg11_eac = 0x9273,
		e_primitive_restart_fixed_index = 0x8D69,
		e_any_samples_passed_conservative = 0x8D6A,
		e_max_element_index = 0x8D6B,
		e_compute_shader = 0x91B9,
		e_max_compute_uniform_blocks = 0x91BB,
		e_max_compute_texture_image_units = 0x91BC,
		e_max_compute_image_uniforms = 0x91BD,
		e_max_compute_shared_memory_size = 0x8262,
		e_max_compute_uniform_components = 0x8263,
		e_max_compute_atomic_counter_buffers = 0x8264,
		e_max_compute_atomic_counters = 0x8265,
		e_max_combined_compute_uniform_components = 0x8266,
		e_max_compute_work_group_invocations = 0x90EB,
		e_max_compute_work_group_count = 0x91BE,
		e_max_compute_work_group_size = 0x91BF,
		e_compute_work_group_size = 0x8267,
		e_uniform_block_referenced_by_compute_shader = 0x90EC,
		e_atomic_counter_buffer_referenced_by_compute_shader = 0x90ED,
		e_dispatch_indirect_buffer = 0x90EE,
		e_dispatch_indirect_buffer_binding = 0x90EF,
		e_compute_shader_bit = 0x00000020,
		e_debug_output_synchronous = 0x8242,
		e_debug_next_logged_message_length = 0x8243,
		e_debug_callback_function = 0x8244,
		e_debug_callback_user_param = 0x8245,
		e_debug_source_api = 0x8246,
		e_debug_source_window_system = 0x8247,
		e_debug_source_shader_compiler = 0x8248,
		e_debug_source_third_party = 0x8249,
		e_debug_source_application = 0x824A,
		e_debug_source_other = 0x824B,
		e_debug_type_error = 0x824C,
		e_debug_type_deprecated_behavior = 0x824D,
		e_debug_type_undefined_behavior = 0x824E,
		e_debug_type_portability = 0x824F,
		e_debug_type_performance = 0x8250,
		e_debug_type_other = 0x8251,
		e_max_debug_message_length = 0x9143,
		e_max_debug_logged_messages = 0x9144,
		e_debug_logged_messages = 0x9145,
		e_debug_severity_high = 0x9146,
		e_debug_severity_medium = 0x9147,
		e_debug_severity_low = 0x9148,
		e_debug_type_marker = 0x8268,
		e_debug_type_push_group = 0x8269,
		e_debug_type_pop_group = 0x826A,
		e_debug_severity_notification = 0x826B,
		e_max_debug_group_stack_depth = 0x826C,
		e_debug_group_stack_depth = 0x826D,
		e_buffer = 0x82E0,
		e_shader = 0x82E1,
		e_program = 0x82E2,
		e_query = 0x82E3,
		e_program_pipeline = 0x82E4,
		e_sampler = 0x82E6,
		e_max_label_length = 0x82E8,
		e_debug_output = 0x92E0,
		e_context_flag_debug_bit = 0x00000002,
		e_max_uniform_locations = 0x826E,
		e_framebuffer_default_width = 0x9310,
		e_framebuffer_default_height = 0x9311,
		e_framebuffer_default_layers = 0x9312,
		e_framebuffer_default_samples = 0x9313,
		e_framebuffer_default_fixed_sample_locations = 0x9314,
		e_max_framebuffer_width = 0x9315,
		e_max_framebuffer_height = 0x9316,
		e_max_framebuffer_layers = 0x9317,
		e_max_framebuffer_samples = 0x9318,
		e_internalformat_supported = 0x826F,
		e_internalformat_preferred = 0x8270,
		e_internalformat_red_size = 0x8271,
		e_internalformat_green_size = 0x8272,
		e_internalformat_blue_size = 0x8273,
		e_internalformat_alpha_size = 0x8274,
		e_internalformat_depth_size = 0x8275,
		e_internalformat_stencil_size = 0x8276,
		e_internalformat_shared_size = 0x8277,
		e_internalformat_red_type = 0x8278,
		e_internalformat_green_type = 0x8279,
		e_internalformat_blue_type = 0x827A,
		e_internalformat_alpha_type = 0x827B,
		e_internalformat_depth_type = 0x827C,
		e_internalformat_stencil_type = 0x827D,
		e_max_width = 0x827E,
		e_max_height = 0x827F,
		e_max_depth = 0x8280,
		e_max_layers = 0x8281,
		e_max_combined_dimensions = 0x8282,
		e_color_components = 0x8283,
		e_depth_components = 0x8284,
		e_stencil_components = 0x8285,
		e_color_renderable = 0x8286,
		e_depth_renderable = 0x8287,
		e_stencil_renderable = 0x8288,
		e_framebuffer_renderable = 0x8289,
		e_framebuffer_renderable_layered = 0x828A,
		e_framebuffer_blend = 0x828B,
		e_read_pixels = 0x828C,
		e_read_pixels_format = 0x828D,
		e_read_pixels_type = 0x828E,
		e_texture_image_format = 0x828F,
		e_texture_image_type = 0x8290,
		e_get_texture_image_format = 0x8291,
		e_get_texture_image_type = 0x8292,
		e_mipmap = 0x8293,
		e_manual_generate_mipmap = 0x8294,
		e_auto_generate_mipmap = 0x8295,
		e_color_encoding = 0x8296,
		e_srgb_read = 0x8297,
		e_srgb_write = 0x8298,
		e_filter = 0x829A,
		e_vertex_texture = 0x829B,
		e_tess_control_texture = 0x829C,
		e_tess_evaluation_texture = 0x829D,
		e_geometry_texture = 0x829E,
		e_fragment_texture = 0x829F,
		e_compute_texture = 0x82A0,
		e_texture_shadow = 0x82A1,
		e_texture_gather = 0x82A2,
		e_texture_gather_shadow = 0x82A3,
		e_shader_image_load = 0x82A4,
		e_shader_image_store = 0x82A5,
		e_shader_image_atomic = 0x82A6,
		e_image_texel_size = 0x82A7,
		e_image_compatibility_class = 0x82A8,
		e_image_pixel_format = 0x82A9,
		e_image_pixel_type = 0x82AA,
		e_simultaneous_texture_and_depth_test = 0x82AC,
		e_simultaneous_texture_and_stencil_test = 0x82AD,
		e_simultaneous_texture_and_depth_write = 0x82AE,
		e_simultaneous_texture_and_stencil_write = 0x82AF,
		e_texture_compressed_block_width = 0x82B1,
		e_texture_compressed_block_height = 0x82B2,
		e_texture_compressed_block_size = 0x82B3,
		e_clear_buffer = 0x82B4,
		e_texture_view = 0x82B5,
		e_view_compatibility_class = 0x82B6,
		e_full_support = 0x82B7,
		e_caveat_support = 0x82B8,
		e_image_class_4_x_32 = 0x82B9,
		e_image_class_2_x_32 = 0x82BA,
		e_image_class_1_x_32 = 0x82BB,
		e_image_class_4_x_16 = 0x82BC,
		e_image_class_2_x_16 = 0x82BD,
		e_image_class_1_x_16 = 0x82BE,
		e_image_class_4_x_8 = 0x82BF,
		e_image_class_2_x_8 = 0x82C0,
		e_image_class_1_x_8 = 0x82C1,
		e_image_class_11_11_10 = 0x82C2,
		e_image_class_10_10_10_2 = 0x82C3,
		e_view_class_128_bits = 0x82C4,
		e_view_class_96_bits = 0x82C5,
		e_view_class_64_bits = 0x82C6,
		e_view_class_48_bits = 0x82C7,
		e_view_class_32_bits = 0x82C8,
		e_view_class_24_bits = 0x82C9,
		e_view_class_16_bits = 0x82CA,
		e_view_class_8_bits = 0x82CB,
		e_view_class_s3tc_dxt1_rgb = 0x82CC,
		e_view_class_s3tc_dxt1_rgba = 0x82CD,
		e_view_class_s3tc_dxt3_rgba = 0x82CE,
		e_view_class_s3tc_dxt5_rgba = 0x82CF,
		e_view_class_rgtc1_red = 0x82D0,
		e_view_class_rgtc2_rg = 0x82D1,
		e_view_class_bptc_unorm = 0x82D2,
		e_view_class_bptc_float = 0x82D3,
		e_uniform = 0x92E1,
		e_uniform_block = 0x92E2,
		e_program_input = 0x92E3,
		e_program_output = 0x92E4,
		e_buffer_variable = 0x92E5,
		e_shader_storage_block = 0x92E6,
		e_vertex_subroutine = 0x92E8,
		e_tess_control_subroutine = 0x92E9,
		e_tess_evaluation_subroutine = 0x92EA,
		e_geometry_subroutine = 0x92EB,
		e_fragment_subroutine = 0x92EC,
		e_compute_subroutine = 0x92ED,
		e_vertex_subroutine_uniform = 0x92EE,
		e_tess_control_subroutine_uniform = 0x92EF,
		e_tess_evaluation_subroutine_uniform = 0x92F0,
		e_geometry_subroutine_uniform = 0x92F1,
		e_fragment_subroutine_uniform = 0x92F2,
		e_compute_subroutine_uniform = 0x92F3,
		e_transform_feedback_varying = 0x92F4,
		e_active_resources = 0x92F5,
		e_max_name_length = 0x92F6,
		e_max_num_active_variables = 0x92F7,
		e_max_num_compatible_subroutines = 0x92F8,
		e_name_length = 0x92F9,
		e_type = 0x92FA,
		e_array_size = 0x92FB,
		e_offset = 0x92FC,
		e_block_index = 0x92FD,
		e_array_stride = 0x92FE,
		e_matrix_stride = 0x92FF,
		e_is_row_major = 0x9300,
		e_atomic_counter_buffer_index = 0x9301,
		e_buffer_binding = 0x9302,
		e_buffer_data_size = 0x9303,
		e_num_active_variables = 0x9304,
		e_active_variables = 0x9305,
		e_referenced_by_vertex_shader = 0x9306,
		e_referenced_by_tess_control_shader = 0x9307,
		e_referenced_by_tess_evaluation_shader = 0x9308,
		e_referenced_by_geometry_shader = 0x9309,
		e_referenced_by_fragment_shader = 0x930A,
		e_referenced_by_compute_shader = 0x930B,
		e_top_level_array_size = 0x930C,
		e_top_level_array_stride = 0x930D,
		e_location = 0x930E,
		e_location_index = 0x930F,
		e_is_per_patch = 0x92E7,
		e_shader_storage_buffer = 0x90D2,
		e_shader_storage_buffer_binding = 0x90D3,
		e_shader_storage_buffer_start = 0x90D4,
		e_shader_storage_buffer_size = 0x90D5,
		e_max_vertex_shader_storage_blocks = 0x90D6,
		e_max_geometry_shader_storage_blocks = 0x90D7,
		e_max_tess_control_shader_storage_blocks = 0x90D8,
		e_max_tess_evaluation_shader_storage_blocks = 0x90D9,
		e_max_fragment_shader_storage_blocks = 0x90DA,
		e_max_compute_shader_storage_blocks = 0x90DB,
		e_max_combined_shader_storage_blocks = 0x90DC,
		e_max_shader_storage_buffer_bindings = 0x90DD,
		e_max_shader_storage_block_size = 0x90DE,
		e_shader_storage_buffer_offset_alignment = 0x90DF,
		e_shader_storage_barrier_bit = 0x00002000,
		e_max_combined_shader_output_resources = 0x8F39,
		e_depth_stencil_texture_mode = 0x90EA,
		e_texture_buffer_offset = 0x919D,
		e_texture_buffer_size = 0x919E,
		e_texture_buffer_offset_alignment = 0x919F,
		e_texture_view_min_level = 0x82DB,
		e_texture_view_num_levels = 0x82DC,
		e_texture_view_min_layer = 0x82DD,
		e_texture_view_num_layers = 0x82DE,
		e_texture_immutable_levels = 0x82DF,
		e_vertex_attrib_binding = 0x82D4,
		e_vertex_attrib_relative_offset = 0x82D5,
		e_vertex_binding_divisor = 0x82D6,
		e_vertex_binding_offset = 0x82D7,
		e_vertex_binding_stride = 0x82D8,
		e_max_vertex_attrib_relative_offset = 0x82D9,
		e_max_vertex_attrib_bindings = 0x82DA,
		e_vertex_binding_buffer = 0x8F4F,
		e_max_vertex_attrib_stride = 0x82E5,
		e_primitive_restart_for_patches_supported = 0x8221,
		e_texture_buffer_binding = 0x8C2A,
		e_map_persistent_bit = 0x0040,
		e_map_coherent_bit = 0x0080,
		e_dynamic_storage_bit = 0x0100,
		e_client_storage_bit = 0x0200,
		e_client_mapped_buffer_barrier_bit = 0x00004000,
		e_buffer_immutable_storage = 0x821F,
		e_buffer_storage_flags = 0x8220,
		e_clear_texture = 0x9365,
		e_location_component = 0x934A,
		e_transform_feedback_buffer_index = 0x934B,
		e_transform_feedback_buffer_stride = 0x934C,
		e_query_buffer = 0x9192,
		e_query_buffer_barrier_bit = 0x00008000,
		e_query_buffer_binding = 0x9193,
		e_query_result_no_wait = 0x9194,
		e_mirror_clamp_to_edge = 0x8743,
		e_context_lost = 0x0507,
		e_negative_one_to_one = 0x935E,
		e_zero_to_one = 0x935F,
		e_clip_origin = 0x935C,
		e_clip_depth_mode = 0x935D,
		e_query_wait_inverted = 0x8E17,
		e_query_no_wait_inverted = 0x8E18,
		e_query_by_region_wait_inverted = 0x8E19,
		e_query_by_region_no_wait_inverted = 0x8E1A,
		e_max_cull_distances = 0x82F9,
		e_max_combined_clip_and_cull_distances = 0x82FA,
		e_texture_target = 0x1006,
		e_query_target = 0x82EA,
		e_guilty_context_reset = 0x8253,
		e_innocent_context_reset = 0x8254,
		e_unknown_context_reset = 0x8255,
		e_reset_notification_strategy = 0x8256,
		e_lose_context_on_reset = 0x8252,
		e_no_reset_notification = 0x8261,
		e_context_flag_robust_access_bit = 0x00000004,
		e_context_release_behavior = 0x82FB,
		e_context_release_behavior_flush = 0x82FC,
		e_shader_binary_format_spir_v = 0x9551,
		e_spir_v_binary = 0x9552,
		e_parameter_buffer = 0x80EE,
		e_parameter_buffer_binding = 0x80EF,
		e_context_flag_no_error_bit = 0x00000008,
		e_vertices_submitted = 0x82EE,
		e_primitives_submitted = 0x82EF,
		e_vertex_shader_invocations = 0x82F0,
		e_tess_control_shader_patches = 0x82F1,
		e_tess_evaluation_shader_invocations = 0x82F2,
		e_geometry_shader_primitives_emitted = 0x82F3,
		e_fragment_shader_invocations = 0x82F4,
		e_compute_shader_invocations = 0x82F5,
		e_clipping_input_primitives = 0x82F6,
		e_clipping_output_primitives = 0x82F7,
		e_polygon_offset_clamp = 0x8E1B,
		e_spir_v_extensions = 0x9553,
		e_num_spir_v_extensions = 0x9554,
		e_texture_max_anisotropy = 0x84FE,
		e_max_texture_max_anisotropy = 0x84FF,
		e_transform_feedback_overflow = 0x82EC,
		e_transform_feedback_stream_overflow = 0x82ED,
		e_primitive_bounding_box_arb = 0x92BE,
		e_multisample_line_width_range_arb = 0x9381,
		e_multisample_line_width_granularity_arb = 0x9382,
		e_unsigned_int64_arb = 0x140F,
		e_sync_cl_event_arb = 0x8240,
		e_sync_cl_event_complete_arb = 0x8241,
		e_max_compute_variable_group_invocations_arb = 0x9344,
		e_max_compute_fixed_group_invocations_arb = 0x90EB,
		e_max_compute_variable_group_size_arb = 0x9345,
		e_max_compute_fixed_group_size_arb = 0x91BF,
		e_debug_output_synchronous_arb = 0x8242,
		e_debug_next_logged_message_length_arb = 0x8243,
		e_debug_callback_function_arb = 0x8244,
		e_debug_callback_user_param_arb = 0x8245,
		e_debug_source_api_arb = 0x8246,
		e_debug_source_window_system_arb = 0x8247,
		e_debug_source_shader_compiler_arb = 0x8248,
		e_debug_source_third_party_arb = 0x8249,
		e_debug_source_application_arb = 0x824A,
		e_debug_source_other_arb = 0x824B,
		e_debug_type_error_arb = 0x824C,
		e_debug_type_deprecated_behavior_arb = 0x824D,
		e_debug_type_undefined_behavior_arb = 0x824E,
		e_debug_type_portability_arb = 0x824F,
		e_debug_type_performance_arb = 0x8250,
		e_debug_type_other_arb = 0x8251,
		e_max_debug_message_length_arb = 0x9143,
		e_max_debug_logged_messages_arb = 0x9144,
		e_debug_logged_messages_arb = 0x9145,
		e_debug_severity_high_arb = 0x9146,
		e_debug_severity_medium_arb = 0x9147,
		e_debug_severity_low_arb = 0x9148,
		e_lines_adjacency_arb = 0x000A,
		e_line_strip_adjacency_arb = 0x000B,
		e_triangles_adjacency_arb = 0x000C,
		e_triangle_strip_adjacency_arb = 0x000D,
		e_program_point_size_arb = 0x8642,
		e_max_geometry_texture_image_units_arb = 0x8C29,
		e_framebuffer_attachment_layered_arb = 0x8DA7,
		e_framebuffer_incomplete_layer_targets_arb = 0x8DA8,
		e_framebuffer_incomplete_layer_count_arb = 0x8DA9,
		e_geometry_shader_arb = 0x8DD9,
		e_geometry_vertices_out_arb = 0x8DDA,
		e_geometry_input_type_arb = 0x8DDB,
		e_geometry_output_type_arb = 0x8DDC,
		e_max_geometry_varying_components_arb = 0x8DDD,
		e_max_vertex_varying_components_arb = 0x8DDE,
		e_max_geometry_uniform_components_arb = 0x8DDF,
		e_max_geometry_output_vertices_arb = 0x8DE0,
		e_max_geometry_total_output_components_arb = 0x8DE1,
		e_shader_binary_format_spir_v_arb = 0x9551,
		e_spir_v_binary_arb = 0x9552,
		e_int64_arb = 0x140E,
		e_int64_vec2_arb = 0x8FE9,
		e_int64_vec3_arb = 0x8FEA,
		e_int64_vec4_arb = 0x8FEB,
		e_unsigned_int64_vec2_arb = 0x8FF5,
		e_unsigned_int64_vec3_arb = 0x8FF6,
		e_unsigned_int64_vec4_arb = 0x8FF7,
		e_parameter_buffer_arb = 0x80EE,
		e_parameter_buffer_binding_arb = 0x80EF,
		e_vertex_attrib_array_divisor_arb = 0x88FE,
		e_srgb_decode_arb = 0x8299,
		e_view_class_eac_r11 = 0x9383,
		e_view_class_eac_rg11 = 0x9384,
		e_view_class_etc2_rgb = 0x9385,
		e_view_class_etc2_rgba = 0x9386,
		e_view_class_etc2_eac_rgba = 0x9387,
		e_view_class_astc_4x4_rgba = 0x9388,
		e_view_class_astc_5x4_rgba = 0x9389,
		e_view_class_astc_5x5_rgba = 0x938A,
		e_view_class_astc_6x5_rgba = 0x938B,
		e_view_class_astc_6x6_rgba = 0x938C,
		e_view_class_astc_8x5_rgba = 0x938D,
		e_view_class_astc_8x6_rgba = 0x938E,
		e_view_class_astc_8x8_rgba = 0x938F,
		e_view_class_astc_10x5_rgba = 0x9390,
		e_view_class_astc_10x6_rgba = 0x9391,
		e_view_class_astc_10x8_rgba = 0x9392,
		e_view_class_astc_10x10_rgba = 0x9393,
		e_view_class_astc_12x10_rgba = 0x9394,
		e_view_class_astc_12x12_rgba = 0x9395,
		e_max_shader_compiler_threads_arb = 0x91B0,
		e_completion_status_arb = 0x91B1,
		e_vertices_submitted_arb = 0x82EE,
		e_primitives_submitted_arb = 0x82EF,
		e_vertex_shader_invocations_arb = 0x82F0,
		e_tess_control_shader_patches_arb = 0x82F1,
		e_tess_evaluation_shader_invocations_arb = 0x82F2,
		e_geometry_shader_primitives_emitted_arb = 0x82F3,
		e_fragment_shader_invocations_arb = 0x82F4,
		e_compute_shader_invocations_arb = 0x82F5,
		e_clipping_input_primitives_arb = 0x82F6,
		e_clipping_output_primitives_arb = 0x82F7,
		e_pixel_pack_buffer_arb = 0x88EB,
		e_pixel_unpack_buffer_arb = 0x88EC,
		e_pixel_pack_buffer_binding_arb = 0x88ED,
		e_pixel_unpack_buffer_binding_arb = 0x88EF,
		e_context_flag_robust_access_bit_arb = 0x00000004,
		e_lose_context_on_reset_arb = 0x8252,
		e_guilty_context_reset_arb = 0x8253,
		e_innocent_context_reset_arb = 0x8254,
		e_unknown_context_reset_arb = 0x8255,
		e_reset_notification_strategy_arb = 0x8256,
		e_no_reset_notification_arb = 0x8261,
		e_sample_location_subpixel_bits_arb = 0x933D,
		e_sample_location_pixel_grid_width_arb = 0x933E,
		e_sample_location_pixel_grid_height_arb = 0x933F,
		e_programmable_sample_location_table_size_arb = 0x9340,
		e_sample_location_arb = 0x8E50,
		e_programmable_sample_location_arb = 0x9341,
		e_framebuffer_programmable_sample_locations_arb = 0x9342,
		e_framebuffer_sample_location_pixel_grid_arb = 0x9343,
		e_sample_shading_arb = 0x8C36,
		e_min_sample_shading_value_arb = 0x8C37,
		e_shader_include_arb = 0x8DAE,
		e_named_string_length_arb = 0x8DE9,
		e_named_string_type_arb = 0x8DEA,
		e_sparse_storage_bit_arb = 0x0400,
		e_sparse_buffer_page_size_arb = 0x82F8,
		e_texture_sparse_arb = 0x91A6,
		e_virtual_page_size_index_arb = 0x91A7,
		e_num_sparse_levels_arb = 0x91AA,
		e_num_virtual_page_sizes_arb = 0x91A8,
		e_virtual_page_size_x_arb = 0x9195,
		e_virtual_page_size_y_arb = 0x9196,
		e_virtual_page_size_z_arb = 0x9197,
		e_max_sparse_texture_size_arb = 0x9198,
		e_max_sparse_3d_texture_size_arb = 0x9199,
		e_max_sparse_array_texture_layers_arb = 0x919A,
		e_sparse_texture_full_array_cube_mipmaps_arb = 0x91A9,
		e_clamp_to_border_arb = 0x812D,
		e_texture_buffer_arb = 0x8C2A,
		e_max_texture_buffer_size_arb = 0x8C2B,
		e_texture_binding_buffer_arb = 0x8C2C,
		e_texture_buffer_data_store_binding_arb = 0x8C2D,
		e_texture_buffer_format_arb = 0x8C2E,
		e_compressed_rgba_bptc_unorm_arb = 0x8E8C,
		e_compressed_srgb_alpha_bptc_unorm_arb = 0x8E8D,
		e_compressed_rgb_bptc_signed_float_arb = 0x8E8E,
		e_compressed_rgb_bptc_unsigned_float_arb = 0x8E8F,
		e_texture_cube_map_array_arb = 0x9009,
		e_texture_binding_cube_map_array_arb = 0x900A,
		e_proxy_texture_cube_map_array_arb = 0x900B,
		e_sampler_cube_map_array_arb = 0x900C,
		e_sampler_cube_map_array_shadow_arb = 0x900D,
		e_int_sampler_cube_map_array_arb = 0x900E,
		e_unsigned_int_sampler_cube_map_array_arb = 0x900F,
		e_texture_reduction_mode_arb = 0x9366,
		e_weighted_average_arb = 0x9367,
		e_min_program_texture_gather_offset_arb = 0x8E5E,
		e_max_program_texture_gather_offset_arb = 0x8E5F,
		e_max_program_texture_gather_components_arb = 0x8F9F,
		e_mirrored_repeat_arb = 0x8370,
		e_transform_feedback_overflow_arb = 0x82EC,
		e_transform_feedback_stream_overflow_arb = 0x82ED,
		e_multiply_khr = 0x9294,
		e_screen_khr = 0x9295,
		e_overlay_khr = 0x9296,
		e_darken_khr = 0x9297,
		e_lighten_khr = 0x9298,
		e_colordodge_khr = 0x9299,
		e_colorburn_khr = 0x929A,
		e_hardlight_khr = 0x929B,
		e_softlight_khr = 0x929C,
		e_difference_khr = 0x929E,
		e_exclusion_khr = 0x92A0,
		e_hsl_hue_khr = 0x92AD,
		e_hsl_saturation_khr = 0x92AE,
		e_hsl_color_khr = 0x92AF,
		e_hsl_luminosity_khr = 0x92B0,
		e_blend_advanced_coherent_khr = 0x9285,
		e_context_flag_no_error_bit_khr = 0x00000008,
		e_max_shader_compiler_threads_khr = 0x91B0,
		e_completion_status_khr = 0x91B1,
		e_context_robust_access = 0x90F3,
		e_subgroup_size_khr = 0x9532,
		e_subgroup_supported_stages_khr = 0x9533,
		e_subgroup_supported_features_khr = 0x9534,
		e_subgroup_quad_all_stages_khr = 0x9535,
		e_subgroup_feature_basic_bit_khr = 0x00000001,
		e_subgroup_feature_vote_bit_khr = 0x00000002,
		e_subgroup_feature_arithmetic_bit_khr = 0x00000004,
		e_subgroup_feature_ballot_bit_khr = 0x00000008,
		e_subgroup_feature_shuffle_bit_khr = 0x00000010,
		e_subgroup_feature_shuffle_relative_bit_khr = 0x00000020,
		e_subgroup_feature_clustered_bit_khr = 0x00000040,
		e_subgroup_feature_quad_bit_khr = 0x00000080,
		e_compressed_rgba_astc_4x4_khr = 0x93B0,
		e_compressed_rgba_astc_5x4_khr = 0x93B1,
		e_compressed_rgba_astc_5x5_khr = 0x93B2,
		e_compressed_rgba_astc_6x5_khr = 0x93B3,
		e_compressed_rgba_astc_6x6_khr = 0x93B4,
		e_compressed_rgba_astc_8x5_khr = 0x93B5,
		e_compressed_rgba_astc_8x6_khr = 0x93B6,
		e_compressed_rgba_astc_8x8_khr = 0x93B7,
		e_compressed_rgba_astc_10x5_khr = 0x93B8,
		e_compressed_rgba_astc_10x6_khr = 0x93B9,
		e_compressed_rgba_astc_10x8_khr = 0x93BA,
		e_compressed_rgba_astc_10x10_khr = 0x93BB,
		e_compressed_rgba_astc_12x10_khr = 0x93BC,
		e_compressed_rgba_astc_12x12_khr = 0x93BD,
		e_compressed_srgb8_alpha8_astc_4x4_khr = 0x93D0,
		e_compressed_srgb8_alpha8_astc_5x4_khr = 0x93D1,
		e_compressed_srgb8_alpha8_astc_5x5_khr = 0x93D2,
		e_compressed_srgb8_alpha8_astc_6x5_khr = 0x93D3,
		e_compressed_srgb8_alpha8_astc_6x6_khr = 0x93D4,
		e_compressed_srgb8_alpha8_astc_8x5_khr = 0x93D5,
		e_compressed_srgb8_alpha8_astc_8x6_khr = 0x93D6,
		e_compressed_srgb8_alpha8_astc_8x8_khr = 0x93D7,
		e_compressed_srgb8_alpha8_astc_10x5_khr = 0x93D8,
		e_compressed_srgb8_alpha8_astc_10x6_khr = 0x93D9,
		e_compressed_srgb8_alpha8_astc_10x8_khr = 0x93DA,
		e_compressed_srgb8_alpha8_astc_10x10_khr = 0x93DB,
		e_compressed_srgb8_alpha8_astc_12x10_khr = 0x93DC,
		e_compressed_srgb8_alpha8_astc_12x12_khr = 0x93DD,
		e_renderbuffer_storage_samples_amd = 0x91B2,
		e_max_color_framebuffer_samples_amd = 0x91B3,
		e_max_color_framebuffer_storage_samples_amd = 0x91B4,
		e_max_depth_stencil_framebuffer_samples_amd = 0x91B5,
		e_num_supported_multisample_modes_amd = 0x91B6,
		e_supported_multisample_modes_amd = 0x91B7,
		e_counter_type_amd = 0x8BC0,
		e_counter_range_amd = 0x8BC1,
		e_unsigned_int64_amd = 0x8BC2,
		e_percentage_amd = 0x8BC3,
		e_perfmon_result_available_amd = 0x8BC4,
		e_perfmon_result_size_amd = 0x8BC5,
		e_perfmon_result_amd = 0x8BC6,
		e_rgb_422_apple = 0x8A1F,
		e_unsigned_short_8_8_apple = 0x85BA,
		e_unsigned_short_8_8_rev_apple = 0x85BB,
		e_rgb_raw_422_apple = 0x8A51,
		e_program_pipeline_object_ext = 0x8A4F,
		e_program_object_ext = 0x8B40,
		e_shader_object_ext = 0x8B48,
		e_buffer_object_ext = 0x9151,
		e_query_object_ext = 0x9153,
		e_vertex_array_object_ext = 0x9154,
		e_program_matrix_ext = 0x8E2D,
		e_transpose_program_matrix_ext = 0x8E2E,
		e_program_matrix_stack_depth_ext = 0x8E2F,
		e_polygon_offset_clamp_ext = 0x8E1B,
		e_raster_multisample_ext = 0x9327,
		e_raster_samples_ext = 0x9328,
		e_max_raster_samples_ext = 0x9329,
		e_raster_fixed_sample_locations_ext = 0x932A,
		e_multisample_rasterization_allowed_ext = 0x932B,
		e_effective_raster_samples_ext = 0x932C,
		e_active_program_ext = 0x8B8D,
		e_fragment_shader_discards_samples_ext = 0x8A52,
		e_compressed_rgb_s3tc_dxt1_ext = 0x83F0,
		e_compressed_rgba_s3tc_dxt1_ext = 0x83F1,
		e_compressed_rgba_s3tc_dxt3_ext = 0x83F2,
		e_compressed_rgba_s3tc_dxt5_ext = 0x83F3,
		e_texture_reduction_mode_ext = 0x9366,
		e_weighted_average_ext = 0x9367,
		e_sr8_ext = 0x8FBD,
		e_srg8_ext = 0x8FBE,
		e_texture_srgb_decode_ext = 0x8A48,
		e_decode_ext = 0x8A49,
		e_skip_decode_ext = 0x8A4A,
		e_texture_immutable_format_ext = 0x912F,
		e_alpha8_ext = 0x803C,
		e_luminance8_ext = 0x8040,
		e_luminance8_alpha8_ext = 0x8045,
		e_rgba32f_ext = 0x8814,
		e_rgb32f_ext = 0x8815,
		e_alpha32f_ext = 0x8816,
		e_luminance32f_ext = 0x8818,
		e_luminance_alpha32f_ext = 0x8819,
		e_rgba16f_ext = 0x881A,
		e_rgb16f_ext = 0x881B,
		e_alpha16f_ext = 0x881C,
		e_luminance16f_ext = 0x881E,
		e_luminance_alpha16f_ext = 0x881F,
		e_rgb10_a2_ext = 0x8059,
		e_rgb10_ext = 0x8052,
		e_bgra8_ext = 0x93A1,
		e_r8_ext = 0x8229,
		e_rg8_ext = 0x822B,
		e_r32f_ext = 0x822E,
		e_rg32f_ext = 0x8230,
		e_r16f_ext = 0x822D,
		e_rg16f_ext = 0x822F,
		e_inclusive_ext = 0x8F10,
		e_exclusive_ext = 0x8F11,
		e_window_rectangle_ext = 0x8F12,
		e_window_rectangle_mode_ext = 0x8F13,
		e_max_window_rectangles_ext = 0x8F14,
		e_num_window_rectangles_ext = 0x8F15,
		e_blackhole_render_intel = 0x83FC,
		e_conservative_rasterization_intel = 0x83FE,
		e_perfquery_single_context_intel = 0x00000000,
		e_perfquery_global_context_intel = 0x00000001,
		e_perfquery_wait_intel = 0x83FB,
		e_perfquery_flush_intel = 0x83FA,
		e_perfquery_donot_flush_intel = 0x83F9,
		e_perfquery_counter_event_intel = 0x94F0,
		e_perfquery_counter_duration_norm_intel = 0x94F1,
		e_perfquery_counter_duration_raw_intel = 0x94F2,
		e_perfquery_counter_throughput_intel = 0x94F3,
		e_perfquery_counter_raw_intel = 0x94F4,
		e_perfquery_counter_timestamp_intel = 0x94F5,
		e_perfquery_counter_data_uint32_intel = 0x94F8,
		e_perfquery_counter_data_uint64_intel = 0x94F9,
		e_perfquery_counter_data_float_intel = 0x94FA,
		e_perfquery_counter_data_double_intel = 0x94FB,
		e_perfquery_counter_data_bool32_intel = 0x94FC,
		e_perfquery_query_name_length_max_intel = 0x94FD,
		e_perfquery_counter_name_length_max_intel = 0x94FE,
		e_perfquery_counter_desc_length_max_intel = 0x94FF,
		e_perfquery_gpa_extended_counters_intel = 0x9500,
		e_framebuffer_flip_x_mesa = 0x8BBC,
		e_framebuffer_flip_y_mesa = 0x8BBB,
		e_framebuffer_swap_xy_mesa = 0x8BBD,
		e_blend_overlap_nv = 0x9281,
		e_blend_premultiplied_src_nv = 0x9280,
		e_blue_nv = 0x1905,
		e_colorburn_nv = 0x929A,
		e_colordodge_nv = 0x9299,
		e_conjoint_nv = 0x9284,
		e_contrast_nv = 0x92A1,
		e_darken_nv = 0x9297,
		e_difference_nv = 0x929E,
		e_disjoint_nv = 0x9283,
		e_dst_atop_nv = 0x928F,
		e_dst_in_nv = 0x928B,
		e_dst_nv = 0x9287,
		e_dst_out_nv = 0x928D,
		e_dst_over_nv = 0x9289,
		e_exclusion_nv = 0x92A0,
		e_green_nv = 0x1904,
		e_hardlight_nv = 0x929B,
		e_hardmix_nv = 0x92A9,
		e_hsl_color_nv = 0x92AF,
		e_hsl_hue_nv = 0x92AD,
		e_hsl_luminosity_nv = 0x92B0,
		e_hsl_saturation_nv = 0x92AE,
		e_invert_ovg_nv = 0x92B4,
		e_invert_rgb_nv = 0x92A3,
		e_lighten_nv = 0x9298,
		e_linearburn_nv = 0x92A5,
		e_lineardodge_nv = 0x92A4,
		e_linearlight_nv = 0x92A7,
		e_minus_clamped_nv = 0x92B3,
		e_minus_nv = 0x929F,
		e_multiply_nv = 0x9294,
		e_overlay_nv = 0x9296,
		e_pinlight_nv = 0x92A8,
		e_plus_clamped_alpha_nv = 0x92B2,
		e_plus_clamped_nv = 0x92B1,
		e_plus_darker_nv = 0x9292,
		e_plus_nv = 0x9291,
		e_red_nv = 0x1903,
		e_screen_nv = 0x9295,
		e_softlight_nv = 0x929C,
		e_src_atop_nv = 0x928E,
		e_src_in_nv = 0x928A,
		e_src_nv = 0x9286,
		e_src_out_nv = 0x928C,
		e_src_over_nv = 0x9288,
		e_uncorrelated_nv = 0x9282,
		e_vividlight_nv = 0x92A6,
		e_xor_nv = 0x1506,
		e_blend_advanced_coherent_nv = 0x9285,
		e_factor_min_amd = 0x901C,
		e_factor_max_amd = 0x901D,
		e_viewport_position_w_scale_nv = 0x937C,
		e_viewport_position_w_scale_x_coeff_nv = 0x937D,
		e_viewport_position_w_scale_y_coeff_nv = 0x937E,
		e_terminate_sequence_command_nv = 0x0000,
		e_nop_command_nv = 0x0001,
		e_draw_elements_command_nv = 0x0002,
		e_draw_arrays_command_nv = 0x0003,
		e_draw_elements_strip_command_nv = 0x0004,
		e_draw_arrays_strip_command_nv = 0x0005,
		e_draw_elements_instanced_command_nv = 0x0006,
		e_draw_arrays_instanced_command_nv = 0x0007,
		e_element_address_command_nv = 0x0008,
		e_attribute_address_command_nv = 0x0009,
		e_uniform_address_command_nv = 0x000A,
		e_blend_color_command_nv = 0x000B,
		e_stencil_ref_command_nv = 0x000C,
		e_line_width_command_nv = 0x000D,
		e_polygon_offset_command_nv = 0x000E,
		e_alpha_ref_command_nv = 0x000F,
		e_viewport_command_nv = 0x0010,
		e_scissor_command_nv = 0x0011,
		e_front_face_command_nv = 0x0012,
		e_query_wait_nv = 0x8E13,
		e_query_no_wait_nv = 0x8E14,
		e_query_by_region_wait_nv = 0x8E15,
		e_query_by_region_no_wait_nv = 0x8E16,
		e_conservative_rasterization_nv = 0x9346,
		e_subpixel_precision_bias_x_bits_nv = 0x9347,
		e_subpixel_precision_bias_y_bits_nv = 0x9348,
		e_max_subpixel_precision_bias_bits_nv = 0x9349,
		e_conservative_raster_dilate_nv = 0x9379,
		e_conservative_raster_dilate_range_nv = 0x937A,
		e_conservative_raster_dilate_granularity_nv = 0x937B,
		e_conservative_raster_mode_pre_snap_nv = 0x9550,
		e_conservative_raster_mode_nv = 0x954D,
		e_conservative_raster_mode_post_snap_nv = 0x954E,
		e_conservative_raster_mode_pre_snap_triangles_nv = 0x954F,
		e_depth_component32f_nv = 0x8DAB,
		e_depth32f_stencil8_nv = 0x8DAC,
		e_float_32_unsigned_int_24_8_rev_nv = 0x8DAD,
		e_depth_buffer_float_mode_nv = 0x8DAF,
		e_fill_rectangle_nv = 0x933C,
		e_fragment_coverage_to_color_nv = 0x92DD,
		e_fragment_coverage_color_nv = 0x92DE,
		e_coverage_modulation_table_nv = 0x9331,
		e_color_samples_nv = 0x8E20,
		e_depth_samples_nv = 0x932D,
		e_stencil_samples_nv = 0x932E,
		e_mixed_depth_samples_supported_nv = 0x932F,
		e_mixed_stencil_samples_supported_nv = 0x9330,
		e_coverage_modulation_nv = 0x9332,
		e_coverage_modulation_table_size_nv = 0x9333,
		e_renderbuffer_coverage_samples_nv = 0x8CAB,
		e_renderbuffer_color_samples_nv = 0x8E10,
		e_max_multisample_coverage_modes_nv = 0x8E11,
		e_multisample_coverage_modes_nv = 0x8E12,
		e_int64_nv = 0x140E,
		e_unsigned_int64_nv = 0x140F,
		e_int8_nv = 0x8FE0,
		e_int8_vec2_nv = 0x8FE1,
		e_int8_vec3_nv = 0x8FE2,
		e_int8_vec4_nv = 0x8FE3,
		e_int16_nv = 0x8FE4,
		e_int16_vec2_nv = 0x8FE5,
		e_int16_vec3_nv = 0x8FE6,
		e_int16_vec4_nv = 0x8FE7,
		e_int64_vec2_nv = 0x8FE9,
		e_int64_vec3_nv = 0x8FEA,
		e_int64_vec4_nv = 0x8FEB,
		e_unsigned_int8_nv = 0x8FEC,
		e_unsigned_int8_vec2_nv = 0x8FED,
		e_unsigned_int8_vec3_nv = 0x8FEE,
		e_unsigned_int8_vec4_nv = 0x8FEF,
		e_unsigned_int16_nv = 0x8FF0,
		e_unsigned_int16_vec2_nv = 0x8FF1,
		e_unsigned_int16_vec3_nv = 0x8FF2,
		e_unsigned_int16_vec4_nv = 0x8FF3,
		e_unsigned_int64_vec2_nv = 0x8FF5,
		e_unsigned_int64_vec3_nv = 0x8FF6,
		e_unsigned_int64_vec4_nv = 0x8FF7,
		e_float16_nv = 0x8FF8,
		e_float16_vec2_nv = 0x8FF9,
		e_float16_vec3_nv = 0x8FFA,
		e_float16_vec4_nv = 0x8FFB,
		e_multisamples_nv = 0x9371,
		e_supersample_scale_x_nv = 0x9372,
		e_supersample_scale_y_nv = 0x9373,
		e_conformant_nv = 0x9374,
		e_attached_memory_object_nv = 0x95A4,
		e_attached_memory_offset_nv = 0x95A5,
		e_memory_attachable_alignment_nv = 0x95A6,
		e_memory_attachable_size_nv = 0x95A7,
		e_memory_attachable_nv = 0x95A8,
		e_detached_memory_incarnation_nv = 0x95A9,
		e_detached_textures_nv = 0x95AA,
		e_detached_buffers_nv = 0x95AB,
		e_max_detached_textures_nv = 0x95AC,
		e_max_detached_buffers_nv = 0x95AD,
		e_mesh_shader_nv = 0x9559,
		e_task_shader_nv = 0x955A,
		e_max_mesh_uniform_blocks_nv = 0x8E60,
		e_max_mesh_texture_image_units_nv = 0x8E61,
		e_max_mesh_image_uniforms_nv = 0x8E62,
		e_max_mesh_uniform_components_nv = 0x8E63,
		e_max_mesh_atomic_counter_buffers_nv = 0x8E64,
		e_max_mesh_atomic_counters_nv = 0x8E65,
		e_max_mesh_shader_storage_blocks_nv = 0x8E66,
		e_max_combined_mesh_uniform_components_nv = 0x8E67,
		e_max_task_uniform_blocks_nv = 0x8E68,
		e_max_task_texture_image_units_nv = 0x8E69,
		e_max_task_image_uniforms_nv = 0x8E6A,
		e_max_task_uniform_components_nv = 0x8E6B,
		e_max_task_atomic_counter_buffers_nv = 0x8E6C,
		e_max_task_atomic_counters_nv = 0x8E6D,
		e_max_task_shader_storage_blocks_nv = 0x8E6E,
		e_max_combined_task_uniform_components_nv = 0x8E6F,
		e_max_mesh_work_group_invocations_nv = 0x95A2,
		e_max_task_work_group_invocations_nv = 0x95A3,
		e_max_mesh_total_memory_size_nv = 0x9536,
		e_max_task_total_memory_size_nv = 0x9537,
		e_max_mesh_output_vertices_nv = 0x9538,
		e_max_mesh_output_primitives_nv = 0x9539,
		e_max_task_output_count_nv = 0x953A,
		e_max_draw_mesh_tasks_count_nv = 0x953D,
		e_max_mesh_views_nv = 0x9557,
		e_mesh_output_per_vertex_granularity_nv = 0x92DF,
		e_mesh_output_per_primitive_granularity_nv = 0x9543,
		e_max_mesh_work_group_size_nv = 0x953B,
		e_max_task_work_group_size_nv = 0x953C,
		e_mesh_work_group_size_nv = 0x953E,
		e_task_work_group_size_nv = 0x953F,
		e_mesh_vertices_out_nv = 0x9579,
		e_mesh_primitives_out_nv = 0x957A,
		e_mesh_output_type_nv = 0x957B,
		e_uniform_block_referenced_by_mesh_shader_nv = 0x959C,
		e_uniform_block_referenced_by_task_shader_nv = 0x959D,
		e_referenced_by_mesh_shader_nv = 0x95A0,
		e_referenced_by_task_shader_nv = 0x95A1,
		e_mesh_shader_bit_nv = 0x00000040,
		e_task_shader_bit_nv = 0x00000080,
		e_mesh_subroutine_nv = 0x957C,
		e_task_subroutine_nv = 0x957D,
		e_mesh_subroutine_uniform_nv = 0x957E,
		e_task_subroutine_uniform_nv = 0x957F,
		e_atomic_counter_buffer_referenced_by_mesh_shader_nv = 0x959E,
		e_atomic_counter_buffer_referenced_by_task_shader_nv = 0x959F,
		e_path_format_svg_nv = 0x9070,
		e_path_format_ps_nv = 0x9071,
		e_standard_font_name_nv = 0x9072,
		e_system_font_name_nv = 0x9073,
		e_file_name_nv = 0x9074,
		e_path_stroke_width_nv = 0x9075,
		e_path_end_caps_nv = 0x9076,
		e_path_initial_end_cap_nv = 0x9077,
		e_path_terminal_end_cap_nv = 0x9078,
		e_path_join_style_nv = 0x9079,
		e_path_miter_limit_nv = 0x907A,
		e_path_dash_caps_nv = 0x907B,
		e_path_initial_dash_cap_nv = 0x907C,
		e_path_terminal_dash_cap_nv = 0x907D,
		e_path_dash_offset_nv = 0x907E,
		e_path_client_length_nv = 0x907F,
		e_path_fill_mode_nv = 0x9080,
		e_path_fill_mask_nv = 0x9081,
		e_path_fill_cover_mode_nv = 0x9082,
		e_path_stroke_cover_mode_nv = 0x9083,
		e_path_stroke_mask_nv = 0x9084,
		e_count_up_nv = 0x9088,
		e_count_down_nv = 0x9089,
		e_path_object_bounding_box_nv = 0x908A,
		e_convex_hull_nv = 0x908B,
		e_bounding_box_nv = 0x908D,
		e_translate_x_nv = 0x908E,
		e_translate_y_nv = 0x908F,
		e_translate_2d_nv = 0x9090,
		e_translate_3d_nv = 0x9091,
		e_affine_2d_nv = 0x9092,
		e_affine_3d_nv = 0x9094,
		e_transpose_affine_2d_nv = 0x9096,
		e_transpose_affine_3d_nv = 0x9098,
		e_utf8_nv = 0x909A,
		e_utf16_nv = 0x909B,
		e_bounding_box_of_bounding_boxes_nv = 0x909C,
		e_path_command_count_nv = 0x909D,
		e_path_coord_count_nv = 0x909E,
		e_path_dash_array_count_nv = 0x909F,
		e_path_computed_length_nv = 0x90A0,
		e_path_fill_bounding_box_nv = 0x90A1,
		e_path_stroke_bounding_box_nv = 0x90A2,
		e_square_nv = 0x90A3,
		e_round_nv = 0x90A4,
		e_triangular_nv = 0x90A5,
		e_bevel_nv = 0x90A6,
		e_miter_revert_nv = 0x90A7,
		e_miter_truncate_nv = 0x90A8,
		e_skip_missing_glyph_nv = 0x90A9,
		e_use_missing_glyph_nv = 0x90AA,
		e_path_error_position_nv = 0x90AB,
		e_accum_adjacent_pairs_nv = 0x90AD,
		e_adjacent_pairs_nv = 0x90AE,
		e_first_to_rest_nv = 0x90AF,
		e_path_gen_mode_nv = 0x90B0,
		e_path_gen_coeff_nv = 0x90B1,
		e_path_gen_components_nv = 0x90B3,
		e_path_stencil_func_nv = 0x90B7,
		e_path_stencil_ref_nv = 0x90B8,
		e_path_stencil_value_mask_nv = 0x90B9,
		e_path_stencil_depth_offset_factor_nv = 0x90BD,
		e_path_stencil_depth_offset_units_nv = 0x90BE,
		e_path_cover_depth_func_nv = 0x90BF,
		e_path_dash_offset_reset_nv = 0x90B4,
		e_move_to_resets_nv = 0x90B5,
		e_move_to_continues_nv = 0x90B6,
		e_close_path_nv = 0x00,
		e_move_to_nv = 0x02,
		e_relative_move_to_nv = 0x03,
		e_line_to_nv = 0x04,
		e_relative_line_to_nv = 0x05,
		e_horizontal_line_to_nv = 0x06,
		e_relative_horizontal_line_to_nv = 0x07,
		e_vertical_line_to_nv = 0x08,
		e_relative_vertical_line_to_nv = 0x09,
		e_quadratic_curve_to_nv = 0x0A,
		e_relative_quadratic_curve_to_nv = 0x0B,
		e_cubic_curve_to_nv = 0x0C,
		e_relative_cubic_curve_to_nv = 0x0D,
		e_smooth_quadratic_curve_to_nv = 0x0E,
		e_relative_smooth_quadratic_curve_to_nv = 0x0F,
		e_smooth_cubic_curve_to_nv = 0x10,
		e_relative_smooth_cubic_curve_to_nv = 0x11,
		e_small_ccw_arc_to_nv = 0x12,
		e_relative_small_ccw_arc_to_nv = 0x13,
		e_small_cw_arc_to_nv = 0x14,
		e_relative_small_cw_arc_to_nv = 0x15,
		e_large_ccw_arc_to_nv = 0x16,
		e_relative_large_ccw_arc_to_nv = 0x17,
		e_large_cw_arc_to_nv = 0x18,
		e_relative_large_cw_arc_to_nv = 0x19,
		e_restart_path_nv = 0xF0,
		e_dup_first_cubic_curve_to_nv = 0xF2,
		e_dup_last_cubic_curve_to_nv = 0xF4,
		e_rect_nv = 0xF6,
		e_circular_ccw_arc_to_nv = 0xF8,
		e_circular_cw_arc_to_nv = 0xFA,
		e_circular_tangent_arc_to_nv = 0xFC,
		e_arc_to_nv = 0xFE,
		e_relative_arc_to_nv = 0xFF,
		e_bold_bit_nv = 0x01,
		e_italic_bit_nv = 0x02,
		e_glyph_width_bit_nv = 0x01,
		e_glyph_height_bit_nv = 0x02,
		e_glyph_horizontal_bearing_x_bit_nv = 0x04,
		e_glyph_horizontal_bearing_y_bit_nv = 0x08,
		e_glyph_horizontal_bearing_advance_bit_nv = 0x10,
		e_glyph_vertical_bearing_x_bit_nv = 0x20,
		e_glyph_vertical_bearing_y_bit_nv = 0x40,
		e_glyph_vertical_bearing_advance_bit_nv = 0x80,
		e_glyph_has_kerning_bit_nv = 0x100,
		e_font_x_min_bounds_bit_nv = 0x00010000,
		e_font_y_min_bounds_bit_nv = 0x00020000,
		e_font_x_max_bounds_bit_nv = 0x00040000,
		e_font_y_max_bounds_bit_nv = 0x00080000,
		e_font_units_per_em_bit_nv = 0x00100000,
		e_font_ascender_bit_nv = 0x00200000,
		e_font_descender_bit_nv = 0x00400000,
		e_font_height_bit_nv = 0x00800000,
		e_font_max_advance_width_bit_nv = 0x01000000,
		e_font_max_advance_height_bit_nv = 0x02000000,
		e_font_underline_position_bit_nv = 0x04000000,
		e_font_underline_thickness_bit_nv = 0x08000000,
		e_font_has_kerning_bit_nv = 0x10000000,
		e_rounded_rect_nv = 0xE8,
		e_relative_rounded_rect_nv = 0xE9,
		e_rounded_rect2_nv = 0xEA,
		e_relative_rounded_rect2_nv = 0xEB,
		e_rounded_rect4_nv = 0xEC,
		e_relative_rounded_rect4_nv = 0xED,
		e_rounded_rect8_nv = 0xEE,
		e_relative_rounded_rect8_nv = 0xEF,
		e_relative_rect_nv = 0xF7,
		e_font_glyphs_available_nv = 0x9368,
		e_font_target_unavailable_nv = 0x9369,
		e_font_unavailable_nv = 0x936A,
		e_font_unintelligible_nv = 0x936B,
		e_conic_curve_to_nv = 0x1A,
		e_relative_conic_curve_to_nv = 0x1B,
		e_font_num_glyph_indices_bit_nv = 0x20000000,
		e_standard_font_format_nv = 0x936C,
		e_path_projection_nv = 0x1701,
		e_path_modelview_nv = 0x1700,
		e_path_modelview_stack_depth_nv = 0x0BA3,
		e_path_modelview_matrix_nv = 0x0BA6,
		e_path_max_modelview_stack_depth_nv = 0x0D36,
		e_path_transpose_modelview_matrix_nv = 0x84E3,
		e_path_projection_stack_depth_nv = 0x0BA4,
		e_path_projection_matrix_nv = 0x0BA7,
		e_path_max_projection_stack_depth_nv = 0x0D38,
		e_path_transpose_projection_matrix_nv = 0x84E4,
		e_fragment_input_nv = 0x936D,
		e_shared_edge_nv = 0xC0,
		e_shading_rate_image_per_primitive_nv = 0x95B1,
		e_shading_rate_image_palette_count_nv = 0x95B2,
		e_representative_fragment_test_nv = 0x937F,
		e_sample_location_subpixel_bits_nv = 0x933D,
		e_sample_location_pixel_grid_width_nv = 0x933E,
		e_sample_location_pixel_grid_height_nv = 0x933F,
		e_programmable_sample_location_table_size_nv = 0x9340,
		e_sample_location_nv = 0x8E50,
		e_programmable_sample_location_nv = 0x9341,
		e_framebuffer_programmable_sample_locations_nv = 0x9342,
		e_framebuffer_sample_location_pixel_grid_nv = 0x9343,
		e_scissor_test_exclusive_nv = 0x9555,
		e_scissor_box_exclusive_nv = 0x9556,
		e_buffer_gpu_address_nv = 0x8F1D,
		e_gpu_address_nv = 0x8F34,
		e_max_shader_buffer_address_nv = 0x8F35,
		e_shader_global_access_barrier_bit_nv = 0x00000010,
		e_subgroup_feature_partitioned_bit_nv = 0x00000100,
		e_warp_size_nv = 0x9339,
		e_warps_per_sm_nv = 0x933A,
		e_sm_count_nv = 0x933B,
		e_shading_rate_image_nv = 0x9563,
		e_shading_rate_no_invocations_nv = 0x9564,
		e_shading_rate_1_invocation_per_pixel_nv = 0x9565,
		e_shading_rate_1_invocation_per_1x2_pixels_nv = 0x9566,
		e_shading_rate_1_invocation_per_2x1_pixels_nv = 0x9567,
		e_shading_rate_1_invocation_per_2x2_pixels_nv = 0x9568,
		e_shading_rate_1_invocation_per_2x4_pixels_nv = 0x9569,
		e_shading_rate_1_invocation_per_4x2_pixels_nv = 0x956A,
		e_shading_rate_1_invocation_per_4x4_pixels_nv = 0x956B,
		e_shading_rate_2_invocations_per_pixel_nv = 0x956C,
		e_shading_rate_4_invocations_per_pixel_nv = 0x956D,
		e_shading_rate_8_invocations_per_pixel_nv = 0x956E,
		e_shading_rate_16_invocations_per_pixel_nv = 0x956F,
		e_shading_rate_image_binding_nv = 0x955B,
		e_shading_rate_image_texel_width_nv = 0x955C,
		e_shading_rate_image_texel_height_nv = 0x955D,
		e_shading_rate_image_palette_size_nv = 0x955E,
		e_max_coarse_fragment_samples_nv = 0x955F,
		e_shading_rate_sample_order_default_nv = 0x95AE,
		e_shading_rate_sample_order_pixel_major_nv = 0x95AF,
		e_shading_rate_sample_order_sample_major_nv = 0x95B0,
		e_uniform_buffer_unified_nv = 0x936E,
		e_uniform_buffer_address_nv = 0x936F,
		e_uniform_buffer_length_nv = 0x9370,
		e_vertex_attrib_array_unified_nv = 0x8F1E,
		e_element_array_unified_nv = 0x8F1F,
		e_vertex_attrib_array_address_nv = 0x8F20,
		e_vertex_array_address_nv = 0x8F21,
		e_normal_array_address_nv = 0x8F22,
		e_color_array_address_nv = 0x8F23,
		e_index_array_address_nv = 0x8F24,
		e_texture_coord_array_address_nv = 0x8F25,
		e_edge_flag_array_address_nv = 0x8F26,
		e_secondary_color_array_address_nv = 0x8F27,
		e_fog_coord_array_address_nv = 0x8F28,
		e_element_array_address_nv = 0x8F29,
		e_vertex_attrib_array_length_nv = 0x8F2A,
		e_vertex_array_length_nv = 0x8F2B,
		e_normal_array_length_nv = 0x8F2C,
		e_color_array_length_nv = 0x8F2D,
		e_index_array_length_nv = 0x8F2E,
		e_texture_coord_array_length_nv = 0x8F2F,
		e_edge_flag_array_length_nv = 0x8F30,
		e_secondary_color_array_length_nv = 0x8F31,
		e_fog_coord_array_length_nv = 0x8F32,
		e_element_array_length_nv = 0x8F33,
		e_draw_indirect_unified_nv = 0x8F40,
		e_draw_indirect_address_nv = 0x8F41,
		e_draw_indirect_length_nv = 0x8F42,
		e_viewport_swizzle_positive_x_nv = 0x9350,
		e_viewport_swizzle_negative_x_nv = 0x9351,
		e_viewport_swizzle_positive_y_nv = 0x9352,
		e_viewport_swizzle_negative_y_nv = 0x9353,
		e_viewport_swizzle_positive_z_nv = 0x9354,
		e_viewport_swizzle_negative_z_nv = 0x9355,
		e_viewport_swizzle_positive_w_nv = 0x9356,
		e_viewport_swizzle_negative_w_nv = 0x9357,
		e_viewport_swizzle_x_nv = 0x9358,
		e_viewport_swizzle_y_nv = 0x9359,
		e_viewport_swizzle_z_nv = 0x935A,
		e_viewport_swizzle_w_nv = 0x935B,
		e_framebuffer_attachment_texture_num_views_ovr = 0x9630,
		e_framebuffer_attachment_texture_base_view_index_ovr = 0x9632,
		e_max_views_ovr = 0x9631,
		e_framebuffer_incomplete_view_targets_ovr = 0x9633
	};

	class c_opengl_dll : public memory::c_module {
	public: using c_module::c_module;
		memory::c_module::i_export* find_stored_export(std::string_view _name) override {
			memory::c_module::i_export* finded_export{ c_module::find_stored_export(_name) };
			return finded_export && finded_export->address ? finded_export : nullptr;
		}

		memory::address_t load_export(std::string_view _name) override {
			if(memory::address_t address{ c_module::load_export(_name) }; address) return address;
			return wglGetProcAddress(_name.data());
		}
	} inline opengl32{ "opengl32.dll" };

	template<typename>
	class c_opengl_export;

	template <typename return_t, typename ...args_t>
	class c_opengl_export<return_t(args_t...)> : public memory::c_module::i_export {
	public:
		typedef return_t(__stdcall* prototype_t)(args_t...);

	public:
		c_opengl_export(std::string_view _name) {
			name = _name;
			if(i_export* finded{ opengl32.find_stored_export(_name) }) address = (*finded).address;
			else opengl32.stored_exports.push_back(this);
		}

	public:
		return_t operator()(args_t... args) {
			if(!address) { address = opengl32.load_export(name); }
			if(!address) throw std::runtime_error{ std::format("'{}' export address == nullptr", name.empty() ? "unknown" : name) };
			return ((prototype_t)address)(args...);
		}
	};

	inline c_opengl_export<void(std::uint32_t op, float value)> accum{ "glAccum" };
	inline c_opengl_export<void(std::uint32_t func, float ref)> alpha_func{ "glAlphaFunc" };
	inline c_opengl_export<bool(int n, const std::uint32_t* textures, bool* residences)> are_textures_resident{ "glAreTexturesResident" };
	inline c_opengl_export<void(int i)> array_element{ "glArrayElement" };
	inline c_opengl_export<void(std::uint32_t mode)> begin{ "glBegin" };
	inline c_opengl_export<void(int width, int height, float xorig, float yorig, float xmove, float ymove, const std::uint8_t* bitmap)> bitmap{ "glBitmap" };
	inline c_opengl_export<void(std::uint32_t list)> call_list{ "glCallList" };
	inline c_opengl_export<void(int n, std::uint32_t type, const void* lists)> call_lists{ "glCallLists" };
	inline c_opengl_export<void(float red, float green, float blue, float alpha)> clear_accum{ "glClearAccum" };
	inline c_opengl_export<void(float c)> clear_index{ "glClearIndex" };
	inline c_opengl_export<void(std::uint32_t plane, const double* equation)> clip_plane{ "glClipPlane" };
	inline c_opengl_export<void(std::int8_t red, std::int8_t green, std::int8_t blue)> color3b{ "glColor3b" };
	inline c_opengl_export<void(const std::int8_t* v)> color3bv{ "glColor3bv" };
	inline c_opengl_export<void(double red, double green, double blue)> color3d{ "glColor3d" };
	inline c_opengl_export<void(const double* v)> color3dv{ "glColor3dv" };
	inline c_opengl_export<void(float red, float green, float blue)> color3f{ "glColor3f" };
	inline c_opengl_export<void(const float* v)> color3fv{ "glColor3fv" };
	inline c_opengl_export<void(int red, int green, int blue)> color3i{ "glColor3i" };
	inline c_opengl_export<void(const int* v)> color3iv{ "glColor3iv" };
	inline c_opengl_export<void(short red, short green, short blue)> color3s{ "glColor3s" };
	inline c_opengl_export<void(const short* v)> color3sv{ "glColor3sv" };
	inline c_opengl_export<void(std::uint8_t red, std::uint8_t green, std::uint8_t blue)> color3ub{ "glColor3ub" };
	inline c_opengl_export<void(const std::uint8_t* v)> color3ubv{ "glColor3ubv" };
	inline c_opengl_export<void(std::uint32_t red, std::uint32_t green, std::uint32_t blue)> color3ui{ "glColor3ui" };
	inline c_opengl_export<void(const std::uint32_t* v)> color3uiv{ "glColor3uiv" };
	inline c_opengl_export<void(std::uint16_t red, std::uint16_t green, std::uint16_t blue)> color3us{ "glColor3us" };
	inline c_opengl_export<void(const std::uint16_t* v)> color3usv{ "glColor3usv" };
	inline c_opengl_export<void(std::int8_t red, std::int8_t green, std::int8_t blue, std::int8_t alpha)> color4b{ "glColor4b" };
	inline c_opengl_export<void(const std::int8_t* v)> color4bv{ "glColor4bv" };
	inline c_opengl_export<void(double red, double green, double blue, double alpha)> color4d{ "glColor4d" };
	inline c_opengl_export<void(const double* v)> color4dv{ "glColor4dv" };
	inline c_opengl_export<void(float red, float green, float blue, float alpha)> color4f{ "glColor4f" };
	inline c_opengl_export<void(const float* v)> color4fv{ "glColor4fv" };
	inline c_opengl_export<void(int red, int green, int blue, int alpha)> color4i{ "glColor4i" };
	inline c_opengl_export<void(const int* v)> color4iv{ "glColor4iv" };
	inline c_opengl_export<void(short red, short green, short blue, short alpha)> color4s{ "glColor4s" };
	inline c_opengl_export<void(const short* v)> color4sv{ "glColor4sv" };
	inline c_opengl_export<void(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)> color4ub{ "glColor4ub" };
	inline c_opengl_export<void(const std::uint8_t* v)> color4ubv{ "glColor4ubv" };
	inline c_opengl_export<void(std::uint32_t red, std::uint32_t green, std::uint32_t blue, std::uint32_t alpha)> color4ui{ "glColor4ui" };
	inline c_opengl_export<void(const std::uint32_t* v)> color4uiv{ "glColor4uiv" };
	inline c_opengl_export<void(std::uint16_t red, std::uint16_t green, std::uint16_t blue, std::uint16_t alpha)> color4us{ "glColor4us" };
	inline c_opengl_export<void(const std::uint16_t* v)> color4usv{ "glColor4usv" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t mode)> color_material{ "glColorMaterial" };
	inline c_opengl_export<void(int size, std::uint32_t type, int stride, const void* pointer)> color_pointer{ "glColorPointer" };
	inline c_opengl_export<void(int x, int y, int width, int height, std::uint32_t type)> copy_pixels{ "glCopyPixels" };
	inline c_opengl_export<void(std::uint32_t list, int range)> delete_lists{ "glDeleteLists" };
	inline c_opengl_export<void(std::uint32_t array)> disable_client_state{ "glDisableClientState" };
	inline c_opengl_export<void(int width, int height, std::uint32_t format, std::uint32_t type, const void* pixels)> draw_pixels{ "glDrawPixels" };
	inline c_opengl_export<void(bool flag)> edge_flag{ "glEdgeFlag" };
	inline c_opengl_export<void(int stride, const void* pointer)> edge_flag_pointer{ "glEdgeFlagPointer" };
	inline c_opengl_export<void(const bool* flag)> edge_flagv{ "glEdgeFlagv" };
	inline c_opengl_export<void(std::uint32_t array)> enable_client_state{ "glEnableClientState" };
	inline c_opengl_export<void()> end{ "glEnd" };
	inline c_opengl_export<void()> end_list{ "glEndList" };
	inline c_opengl_export<void(double u)> eval_coord1d{ "glEvalCoord1d" };
	inline c_opengl_export<void(const double* u)> eval_coord1dv{ "glEvalCoord1dv" };
	inline c_opengl_export<void(float u)> eval_coord1f{ "glEvalCoord1f" };
	inline c_opengl_export<void(const float* u)> eval_coord1fv{ "glEvalCoord1fv" };
	inline c_opengl_export<void(double u, double v)> eval_coord2d{ "glEvalCoord2d" };
	inline c_opengl_export<void(const double* u)> eval_coord2dv{ "glEvalCoord2dv" };
	inline c_opengl_export<void(float u, float v)> eval_coord2f{ "glEvalCoord2f" };
	inline c_opengl_export<void(const float* u)> eval_coord2fv{ "glEvalCoord2fv" };
	inline c_opengl_export<void(std::uint32_t mode, int i1, int i2)> eval_mesh1{ "glEvalMesh1" };
	inline c_opengl_export<void(std::uint32_t mode, int i1, int i2, int j1, int j2)> eval_mesh2{ "glEvalMesh2" };
	inline c_opengl_export<void(int i)> eval_point1{ "glEvalPoint1" };
	inline c_opengl_export<void(int i, int j)> eval_point2{ "glEvalPoint2" };
	inline c_opengl_export<void(int size, std::uint32_t type, float* buffer)> feedback_buffer{ "glFeedbackBuffer" };
	inline c_opengl_export<void(std::uint32_t pname, float param)> fogf{ "glFogf" };
	inline c_opengl_export<void(std::uint32_t pname, const float* params)> fogfv{ "glFogfv" };
	inline c_opengl_export<void(std::uint32_t pname, int param)> fogi{ "glFogi" };
	inline c_opengl_export<void(std::uint32_t pname, const int* params)> fogiv{ "glFogiv" };
	inline c_opengl_export<void(double left, double right, double bottom, double top, double z_near, double z_far)> frustum{ "glFrustum" };
	inline c_opengl_export<std::uint32_t(int range)> gen_lists{ "glGenLists" };
	inline c_opengl_export<void(std::uint32_t plane, double* equation)> get_clip_plane{ "glGetClipPlane" };
	inline c_opengl_export<void(std::uint32_t light, std::uint32_t pname, float* params)> get_lightfv{ "glGetLightfv" };
	inline c_opengl_export<void(std::uint32_t light, std::uint32_t pname, int* params)> get_lightiv{ "glGetLightiv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t query, double* v)> get_mapdv{ "glGetMapdv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t query, float* v)> get_mapfv{ "glGetMapfv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t query, int* v)> get_mapiv{ "glGetMapiv" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t pname, float* params)> get_materialfv{ "glGetMaterialfv" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t pname, int* params)> get_materialiv{ "glGetMaterialiv" };
	inline c_opengl_export<void(std::uint32_t map, float* values)> get_pixel_mapfv{ "glGetPixelMapfv" };
	inline c_opengl_export<void(std::uint32_t map, std::uint32_t* values)> get_pixel_mapuiv{ "glGetPixelMapuiv" };
	inline c_opengl_export<void(std::uint32_t map, std::uint16_t* values)> get_pixel_mapusv{ "glGetPixelMapusv" };
	inline c_opengl_export<void(std::uint8_t* mask)> get_polygon_stipple{ "glGetPolygonStipple" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, float* params)> get_tex_envfv{ "glGetTexEnvfv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int* params)> get_tex_enviv{ "glGetTexEnviv" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, double* params)> get_tex_gendv{ "glGetTexGendv" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, float* params)> get_tex_genfv{ "glGetTexGenfv" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, int* params)> get_tex_geniv{ "glGetTexGeniv" };
	inline c_opengl_export<void(std::uint32_t mask)> index_mask{ "glIndexMask" };
	inline c_opengl_export<void(std::uint32_t type, int stride, const void* pointer)> index_pointer{ "glIndexPointer" };
	inline c_opengl_export<void(double c)> indexd{ "glIndexd" };
	inline c_opengl_export<void(const double* c)> indexdv{ "glIndexdv" };
	inline c_opengl_export<void(float c)> indexf{ "glIndexf" };
	inline c_opengl_export<void(const float* c)> indexfv{ "glIndexfv" };
	inline c_opengl_export<void(int c)> indexi{ "glIndexi" };
	inline c_opengl_export<void(const int* c)> indexiv{ "glIndexiv" };
	inline c_opengl_export<void(short c)> indexs{ "glIndexs" };
	inline c_opengl_export<void(const short* c)> indexsv{ "glIndexsv" };
	inline c_opengl_export<void(std::uint8_t c)> indexub{ "glIndexub" };
	inline c_opengl_export<void(const std::uint8_t* c)> indexubv{ "glIndexubv" };
	inline c_opengl_export<void()> init_names{ "glInitNames" };
	inline c_opengl_export<void(std::uint32_t format, int stride, const void* pointer)> interleaved_arrays{ "glInterleavedArrays" };
	inline c_opengl_export<bool(std::uint32_t list)> is_list{ "glIsList" };
	inline c_opengl_export<void(std::uint32_t pname, float param)> light_modelf{ "glLightModelf" };
	inline c_opengl_export<void(std::uint32_t pname, const float* params)> light_modelfv{ "glLightModelfv" };
	inline c_opengl_export<void(std::uint32_t pname, int param)> light_modeli{ "glLightModeli" };
	inline c_opengl_export<void(std::uint32_t pname, const int* params)> light_modeliv{ "glLightModeliv" };
	inline c_opengl_export<void(std::uint32_t light, std::uint32_t pname, float param)> lightf{ "glLightf" };
	inline c_opengl_export<void(std::uint32_t light, std::uint32_t pname, const float* params)> lightfv{ "glLightfv" };
	inline c_opengl_export<void(std::uint32_t light, std::uint32_t pname, int param)> lighti{ "glLighti" };
	inline c_opengl_export<void(std::uint32_t light, std::uint32_t pname, const int* params)> lightiv{ "glLightiv" };
	inline c_opengl_export<void(int factor, std::uint16_t pattern)> line_stipple{ "glLineStipple" };
	inline c_opengl_export<void(std::uint32_t base)> list_base{ "glListBase" };
	inline c_opengl_export<void()> load_identity{ "glLoadIdentity" };
	inline c_opengl_export<void(const double* m)> load_matrixd{ "glLoadMatrixd" };
	inline c_opengl_export<void(const float* m)> load_matrixf{ "glLoadMatrixf" };
	inline c_opengl_export<void(std::uint32_t name)> load_name{ "glLoadName" };
	inline c_opengl_export<void(std::uint32_t target, double u1, double u2, int stride, int order, const double* points)> map1d{ "glMap1d" };
	inline c_opengl_export<void(std::uint32_t target, float u1, float u2, int stride, int order, const float* points)> map1f{ "glMap1f" };
	inline c_opengl_export<void(std::uint32_t target, double u1, double u2, int ustride, int uorder, double v1, double v2, int vstride, int vorder, const double* points)> map2d{ "glMap2d" };
	inline c_opengl_export<void(std::uint32_t target, float u1, float u2, int ustride, int uorder, float v1, float v2, int vstride, int vorder, const float* points)> map2f{ "glMap2f" };
	inline c_opengl_export<void(int un, double u1, double u2)> map_grid1d{ "glMapGrid1d" };
	inline c_opengl_export<void(int un, float u1, float u2)> map_grid1f{ "glMapGrid1f" };
	inline c_opengl_export<void(int un, double u1, double u2, int vn, double v1, double v2)> map_grid2d{ "glMapGrid2d" };
	inline c_opengl_export<void(int un, float u1, float u2, int vn, float v1, float v2)> map_grid2f{ "glMapGrid2f" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t pname, float param)> materialf{ "glMaterialf" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t pname, const float* params)> materialfv{ "glMaterialfv" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t pname, int param)> materiali{ "glMateriali" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t pname, const int* params)> materialiv{ "glMaterialiv" };
	inline c_opengl_export<void(std::uint32_t mode)> matrix_mode{ "glMatrixMode" };
	inline c_opengl_export<void(const double* m)> mult_matrixd{ "glMultMatrixd" };
	inline c_opengl_export<void(const float* m)> mult_matrixf{ "glMultMatrixf" };
	inline c_opengl_export<void(std::uint32_t list, std::uint32_t mode)> new_list{ "glNewList" };
	inline c_opengl_export<void(std::int8_t nx, std::int8_t ny, std::int8_t nz)> normal3b{ "glNormal3b" };
	inline c_opengl_export<void(const std::int8_t* v)> normal3bv{ "glNormal3bv" };
	inline c_opengl_export<void(double nx, double ny, double nz)> normal3d{ "glNormal3d" };
	inline c_opengl_export<void(const double* v)> normal3dv{ "glNormal3dv" };
	inline c_opengl_export<void(float nx, float ny, float nz)> normal3f{ "glNormal3f" };
	inline c_opengl_export<void(const float* v)> normal3fv{ "glNormal3fv" };
	inline c_opengl_export<void(int nx, int ny, int nz)> normal3i{ "glNormal3i" };
	inline c_opengl_export<void(const int* v)> normal3iv{ "glNormal3iv" };
	inline c_opengl_export<void(short nx, short ny, short nz)> normal3s{ "glNormal3s" };
	inline c_opengl_export<void(const short* v)> normal3sv{ "glNormal3sv" };
	inline c_opengl_export<void(std::uint32_t type, int stride, const void* pointer)> normal_pointer{ "glNormalPointer" };
	inline c_opengl_export<void(double left, double right, double bottom, double top, double z_near, double z_far)> ortho{ "glOrtho" };
	inline c_opengl_export<void(float token)> pass_through{ "glPassThrough" };
	inline c_opengl_export<void(std::uint32_t map, int mapsize, const float* values)> pixel_mapfv{ "glPixelMapfv" };
	inline c_opengl_export<void(std::uint32_t map, int mapsize, const std::uint32_t* values)> pixel_mapuiv{ "glPixelMapuiv" };
	inline c_opengl_export<void(std::uint32_t map, int mapsize, const std::uint16_t* values)> pixel_mapusv{ "glPixelMapusv" };
	inline c_opengl_export<void(std::uint32_t pname, float param)> pixel_transferf{ "glPixelTransferf" };
	inline c_opengl_export<void(std::uint32_t pname, int param)> pixel_transferi{ "glPixelTransferi" };
	inline c_opengl_export<void(float xfactor, float yfactor)> pixel_zoom{ "glPixelZoom" };
	inline c_opengl_export<void(const std::uint8_t* mask)> polygon_stipple{ "glPolygonStipple" };
	inline c_opengl_export<void()> pop_attrib{ "glPopAttrib" };
	inline c_opengl_export<void()> pop_client_attrib{ "glPopClientAttrib" };
	inline c_opengl_export<void()> pop_matrix{ "glPopMatrix" };
	inline c_opengl_export<void()> pop_name{ "glPopName" };
	inline c_opengl_export<void(int n, const std::uint32_t* textures, const float* priorities)> prioritize_textures{ "glPrioritizeTextures" };
	inline c_opengl_export<void(std::uint32_t mask)> push_attrib{ "glPushAttrib" };
	inline c_opengl_export<void(std::uint32_t mask)> push_client_attrib{ "glPushClientAttrib" };
	inline c_opengl_export<void()> push_matrix{ "glPushMatrix" };
	inline c_opengl_export<void(std::uint32_t name)> push_name{ "glPushName" };
	inline c_opengl_export<void(double x, double y)> raster_pos2d{ "glRasterPos2d" };
	inline c_opengl_export<void(const double* v)> raster_pos2dv{ "glRasterPos2dv" };
	inline c_opengl_export<void(float x, float y)> raster_pos2f{ "glRasterPos2f" };
	inline c_opengl_export<void(const float* v)> raster_pos2fv{ "glRasterPos2fv" };
	inline c_opengl_export<void(int x, int y)> raster_pos2i{ "glRasterPos2i" };
	inline c_opengl_export<void(const int* v)> raster_pos2iv{ "glRasterPos2iv" };
	inline c_opengl_export<void(short x, short y)> raster_pos2s{ "glRasterPos2s" };
	inline c_opengl_export<void(const short* v)> raster_pos2sv{ "glRasterPos2sv" };
	inline c_opengl_export<void(double x, double y, double z)> raster_pos3d{ "glRasterPos3d" };
	inline c_opengl_export<void(const double* v)> raster_pos3dv{ "glRasterPos3dv" };
	inline c_opengl_export<void(float x, float y, float z)> raster_pos3f{ "glRasterPos3f" };
	inline c_opengl_export<void(const float* v)> raster_pos3fv{ "glRasterPos3fv" };
	inline c_opengl_export<void(int x, int y, int z)> raster_pos3i{ "glRasterPos3i" };
	inline c_opengl_export<void(const int* v)> raster_pos3iv{ "glRasterPos3iv" };
	inline c_opengl_export<void(short x, short y, short z)> raster_pos3s{ "glRasterPos3s" };
	inline c_opengl_export<void(const short* v)> raster_pos3sv{ "glRasterPos3sv" };
	inline c_opengl_export<void(double x, double y, double z, double w)> raster_pos4d{ "glRasterPos4d" };
	inline c_opengl_export<void(const double* v)> raster_pos4dv{ "glRasterPos4dv" };
	inline c_opengl_export<void(float x, float y, float z, float w)> raster_pos4f{ "glRasterPos4f" };
	inline c_opengl_export<void(const float* v)> raster_pos4fv{ "glRasterPos4fv" };
	inline c_opengl_export<void(int x, int y, int z, int w)> raster_pos4i{ "glRasterPos4i" };
	inline c_opengl_export<void(const int* v)> raster_pos4iv{ "glRasterPos4iv" };
	inline c_opengl_export<void(short x, short y, short z, short w)> raster_pos4s{ "glRasterPos4s" };
	inline c_opengl_export<void(const short* v)> raster_pos4sv{ "glRasterPos4sv" };
	inline c_opengl_export<void(double x1, double y1, double x2, double y2)> rectd{ "glRectd" };
	inline c_opengl_export<void(const double* v1, const double* v2)> rectdv{ "glRectdv" };
	inline c_opengl_export<void(float x1, float y1, float x2, float y2)> rectf{ "glRectf" };
	inline c_opengl_export<void(const float* v1, const float* v2)> rectfv{ "glRectfv" };
	inline c_opengl_export<void(int x1, int y1, int x2, int y2)> recti{ "glRecti" };
	inline c_opengl_export<void(const int* v1, const int* v2)> rectiv{ "glRectiv" };
	inline c_opengl_export<void(short x1, short y1, short x2, short y2)> rects{ "glRects" };
	inline c_opengl_export<void(const short* v1, const short* v2)> rectsv{ "glRectsv" };
	inline c_opengl_export<int(std::uint32_t mode)> render_mode{ "glRenderMode" };
	inline c_opengl_export<void(double angle, double x, double y, double z)> rotated{ "glRotated" };
	inline c_opengl_export<void(float angle, float x, float y, float z)> rotatef{ "glRotatef" };
	inline c_opengl_export<void(double x, double y, double z)> scaled{ "glScaled" };
	inline c_opengl_export<void(float x, float y, float z)> scalef{ "glScalef" };
	inline c_opengl_export<void(int size, std::uint32_t* buffer)> select_buffer{ "glSelectBuffer" };
	inline c_opengl_export<void(std::uint32_t mode)> shade_model{ "glShadeModel" };
	inline c_opengl_export<void(double s)> tex_coord1d{ "glTexCoord1d" };
	inline c_opengl_export<void(const double* v)> tex_coord1dv{ "glTexCoord1dv" };
	inline c_opengl_export<void(float s)> tex_coord1f{ "glTexCoord1f" };
	inline c_opengl_export<void(const float* v)> tex_coord1fv{ "glTexCoord1fv" };
	inline c_opengl_export<void(int s)> tex_coord1i{ "glTexCoord1i" };
	inline c_opengl_export<void(const int* v)> tex_coord1iv{ "glTexCoord1iv" };
	inline c_opengl_export<void(short s)> tex_coord1s{ "glTexCoord1s" };
	inline c_opengl_export<void(const short* v)> tex_coord1sv{ "glTexCoord1sv" };
	inline c_opengl_export<void(double s, double t)> tex_coord2d{ "glTexCoord2d" };
	inline c_opengl_export<void(const double* v)> tex_coord2dv{ "glTexCoord2dv" };
	inline c_opengl_export<void(float s, float t)> tex_coord2f{ "glTexCoord2f" };
	inline c_opengl_export<void(const float* v)> tex_coord2fv{ "glTexCoord2fv" };
	inline c_opengl_export<void(int s, int t)> tex_coord2i{ "glTexCoord2i" };
	inline c_opengl_export<void(const int* v)> tex_coord2iv{ "glTexCoord2iv" };
	inline c_opengl_export<void(short s, short t)> tex_coord2s{ "glTexCoord2s" };
	inline c_opengl_export<void(const short* v)> tex_coord2sv{ "glTexCoord2sv" };
	inline c_opengl_export<void(double s, double t, double r)> tex_coord3d{ "glTexCoord3d" };
	inline c_opengl_export<void(const double* v)> tex_coord3dv{ "glTexCoord3dv" };
	inline c_opengl_export<void(float s, float t, float r)> tex_coord3f{ "glTexCoord3f" };
	inline c_opengl_export<void(const float* v)> tex_coord3fv{ "glTexCoord3fv" };
	inline c_opengl_export<void(int s, int t, int r)> tex_coord3i{ "glTexCoord3i" };
	inline c_opengl_export<void(const int* v)> tex_coord3iv{ "glTexCoord3iv" };
	inline c_opengl_export<void(short s, short t, short r)> tex_coord3s{ "glTexCoord3s" };
	inline c_opengl_export<void(const short* v)> tex_coord3sv{ "glTexCoord3sv" };
	inline c_opengl_export<void(double s, double t, double r, double q)> tex_coord4d{ "glTexCoord4d" };
	inline c_opengl_export<void(const double* v)> tex_coord4dv{ "glTexCoord4dv" };
	inline c_opengl_export<void(float s, float t, float r, float q)> tex_coord4f{ "glTexCoord4f" };
	inline c_opengl_export<void(const float* v)> tex_coord4fv{ "glTexCoord4fv" };
	inline c_opengl_export<void(int s, int t, int r, int q)> tex_coord4i{ "glTexCoord4i" };
	inline c_opengl_export<void(const int* v)> tex_coord4iv{ "glTexCoord4iv" };
	inline c_opengl_export<void(short s, short t, short r, short q)> tex_coord4s{ "glTexCoord4s" };
	inline c_opengl_export<void(const short* v)> tex_coord4sv{ "glTexCoord4sv" };
	inline c_opengl_export<void(int size, std::uint32_t type, int stride, const void* pointer)> tex_coord_pointer{ "glTexCoordPointer" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, float param)> tex_envf{ "glTexEnvf" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, const float* params)> tex_envfv{ "glTexEnvfv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int param)> tex_envi{ "glTexEnvi" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, const int* params)> tex_enviv{ "glTexEnviv" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, double param)> tex_gend{ "glTexGend" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, const double* params)> tex_gendv{ "glTexGendv" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, float param)> tex_genf{ "glTexGenf" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, const float* params)> tex_genfv{ "glTexGenfv" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, int param)> tex_geni{ "glTexGeni" };
	inline c_opengl_export<void(std::uint32_t coord, std::uint32_t pname, const int* params)> tex_geniv{ "glTexGeniv" };
	inline c_opengl_export<void(double x, double y, double z)> translated{ "glTranslated" };
	inline c_opengl_export<void(float x, float y, float z)> translatef{ "glTranslatef" };
	inline c_opengl_export<void(double x, double y)> vertex2d{ "glVertex2d" };
	inline c_opengl_export<void(const double* v)> vertex2dv{ "glVertex2dv" };
	inline c_opengl_export<void(float x, float y)> vertex2f{ "glVertex2f" };
	inline c_opengl_export<void(const float* v)> vertex2fv{ "glVertex2fv" };
	inline c_opengl_export<void(int x, int y)> vertex2i{ "glVertex2i" };
	inline c_opengl_export<void(const int* v)> vertex2iv{ "glVertex2iv" };
	inline c_opengl_export<void(short x, short y)> vertex2s{ "glVertex2s" };
	inline c_opengl_export<void(const short* v)> vertex2sv{ "glVertex2sv" };
	inline c_opengl_export<void(double x, double y, double z)> vertex3d{ "glVertex3d" };
	inline c_opengl_export<void(const double* v)> vertex3dv{ "glVertex3dv" };
	inline c_opengl_export<void(float x, float y, float z)> vertex3f{ "glVertex3f" };
	inline c_opengl_export<void(const float* v)> vertex3fv{ "glVertex3fv" };
	inline c_opengl_export<void(int x, int y, int z)> vertex3i{ "glVertex3i" };
	inline c_opengl_export<void(const int* v)> vertex3iv{ "glVertex3iv" };
	inline c_opengl_export<void(short x, short y, short z)> vertex3s{ "glVertex3s" };
	inline c_opengl_export<void(const short* v)> vertex3sv{ "glVertex3sv" };
	inline c_opengl_export<void(double x, double y, double z, double w)> vertex4d{ "glVertex4d" };
	inline c_opengl_export<void(const double* v)> vertex4dv{ "glVertex4dv" };
	inline c_opengl_export<void(float x, float y, float z, float w)> vertex4f{ "glVertex4f" };
	inline c_opengl_export<void(const float* v)> vertex4fv{ "glVertex4fv" };
	inline c_opengl_export<void(int x, int y, int z, int w)> vertex4i{ "glVertex4i" };
	inline c_opengl_export<void(const int* v)> vertex4iv{ "glVertex4iv" };
	inline c_opengl_export<void(short x, short y, short z, short w)> vertex4s{ "glVertex4s" };
	inline c_opengl_export<void(const short* v)> vertex4sv{ "glVertex4sv" };
	inline c_opengl_export<void(int size, std::uint32_t type, int stride, const void* pointer)> vertex_pointer{ "glVertexPointer" };
	inline c_opengl_export<void(std::uint32_t mode)> cull_face{ "glCullFace" };
	inline c_opengl_export<void(std::uint32_t mode)> front_face{ "glFrontFace" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t mode)> hint{ "glHint" };
	inline c_opengl_export<void(float width)> line_width{ "glLineWidth" };
	inline c_opengl_export<void(float size)> point_size{ "glPointSize" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t mode)> polygon_mode{ "glPolygonMode" };
	inline c_opengl_export<void(int x, int y, int width, int height)> scissor{ "glScissor" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, float param)> tex_parameterf{ "glTexParameterf" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, const float* params)> tex_parameterfv{ "glTexParameterfv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int param)> tex_parameteri{ "glTexParameteri" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, const int* params)> tex_parameteriv{ "glTexParameteriv" };
	inline c_opengl_export<void(std::uint32_t target, int level, int internalformat, int width, int border, std::uint32_t format, std::uint32_t type, const void* pixels)> tex_image1d{ "glTexImage1D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int internalformat, int width, int height, int border, std::uint32_t format, std::uint32_t type, const void* pixels)> tex_image2d{ "glTexImage2D" };
	inline c_opengl_export<void(std::uint32_t buf)> draw_buffer{ "glDrawBuffer" };
	inline c_opengl_export<void(std::uint32_t mask)> clear{ "glClear" };
	inline c_opengl_export<void(float red, float green, float blue, float alpha)> clear_color{ "glClearColor" };
	inline c_opengl_export<void(int s)> clear_stencil{ "glClearStencil" };
	inline c_opengl_export<void(double depth)> clear_depth{ "glClearDepth" };
	inline c_opengl_export<void(std::uint32_t mask)> stencil_mask{ "glStencilMask" };
	inline c_opengl_export<void(bool red, bool green, bool blue, bool alpha)> color_mask{ "glColorMask" };
	inline c_opengl_export<void(bool flag)> depth_mask{ "glDepthMask" };
	inline c_opengl_export<void(std::uint32_t cap)> disable{ "glDisable" };
	inline c_opengl_export<void(std::uint32_t cap)> enable{ "glEnable" };
	inline c_opengl_export<void()> finish{ "glFinish" };
	inline c_opengl_export<void()> flush{ "glFlush" };
	inline c_opengl_export<void(std::uint32_t sfactor, std::uint32_t dfactor)> blend_func{ "glBlendFunc" };
	inline c_opengl_export<void(std::uint32_t opcode)> logic_op{ "glLogicOp" };
	inline c_opengl_export<void(std::uint32_t func, int ref, std::uint32_t mask)> stencil_func{ "glStencilFunc" };
	inline c_opengl_export<void(std::uint32_t fail, std::uint32_t zfail, std::uint32_t zpass)> stencil_op{ "glStencilOp" };
	inline c_opengl_export<void(std::uint32_t func)> depth_func{ "glDepthFunc" };
	inline c_opengl_export<void(std::uint32_t pname, float param)> pixel_storef{ "glPixelStoref" };
	inline c_opengl_export<void(std::uint32_t pname, int param)> pixel_storei{ "glPixelStorei" };
	inline c_opengl_export<void(std::uint32_t src)> read_buffer{ "glReadBuffer" };
	inline c_opengl_export<void(int x, int y, int width, int height, std::uint32_t format, std::uint32_t type, void* pixels)> read_pixels{ "glReadPixels" };
	inline c_opengl_export<void(std::uint32_t pname, bool* data)> get_booleanv{ "glGetBooleanv" };
	inline c_opengl_export<void(std::uint32_t pname, double* data)> get_doublev{ "glGetDoublev" };
	inline c_opengl_export<std::uint32_t()> get_error{ "glGetError" };
	inline c_opengl_export<void(std::uint32_t pname, float* data)> get_floatv{ "glGetFloatv" };
	inline c_opengl_export<void(std::uint32_t pname, int* data)> get_integerv{ "glGetIntegerv" };
	inline c_opengl_export<const std::uint8_t*(std::uint32_t name)> get_string{ "glGetString" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t format, std::uint32_t type, void* pixels)> get_tex_image{ "glGetTexImage" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, float* params)> get_tex_parameterfv{ "glGetTexParameterfv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int* params)> get_tex_parameteriv{ "glGetTexParameteriv" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t pname, float* params)> get_tex_level_parameterfv{ "glGetTexLevelParameterfv" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t pname, int* params)> get_tex_level_parameteriv{ "glGetTexLevelParameteriv" };
	inline c_opengl_export<bool(std::uint32_t cap)> is_enabled{ "glIsEnabled" };
	inline c_opengl_export<void(double n, double f)> depth_range{ "glDepthRange" };
	inline c_opengl_export<void(int x, int y, int width, int height)> viewport{ "glViewport" };
	inline c_opengl_export<void(std::uint32_t mode, int first, int count)> draw_arrays{ "glDrawArrays" };
	inline c_opengl_export<void(std::uint32_t mode, int count, std::uint32_t type, const void* indices)> draw_elements{ "glDrawElements" };
	inline c_opengl_export<void(std::uint32_t pname, void** params)> get_pointerv{ "glGetPointerv" };
	inline c_opengl_export<void(float factor, float units)> polygon_offset{ "glPolygonOffset" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t internal_format, int x, int y, int width, int border)> copy_tex_image1d{ "glCopyTexImage1D" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t internal_format, int x, int y, int width, int height, int border)> copy_tex_image2d{ "glCopyTexImage2D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int x, int y, int width)> copy_tex_sub_image1d{ "glCopyTexSubImage1D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int yoffset, int x, int y, int width, int height)> copy_tex_sub_image2d{ "glCopyTexSubImage2D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int width, std::uint32_t format, std::uint32_t type, const void* pixels)> tex_sub_image1d{ "glTexSubImage1D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int yoffset, int width, int height, std::uint32_t format, std::uint32_t type, const void* pixels)> tex_sub_image2d{ "glTexSubImage2D" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t texture)> bind_texture{ "glBindTexture" };
	inline c_opengl_export<void(int n, const std::uint32_t* textures)> delete_textures{ "glDeleteTextures" };
	inline c_opengl_export<void(int n, std::uint32_t* textures)> gen_textures{ "glGenTextures" };
	inline c_opengl_export<bool(std::uint32_t texture)> is_texture{ "glIsTexture" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t start, std::uint32_t end, int count, std::uint32_t type, const void* indices)> draw_range_elements{ "glDrawRangeElements" };
	inline c_opengl_export<void(std::uint32_t target, int level, int internalformat, int width, int height, int depth, int border, std::uint32_t format, std::uint32_t type, const void* pixels)> tex_image3d{ "glTexImage3D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, std::uint32_t format, std::uint32_t type, const void* pixels)> tex_sub_image3d{ "glTexSubImage3D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height)> copy_tex_sub_image3d{ "glCopyTexSubImage3D" };
	inline c_opengl_export<void(std::uint32_t texture)> active_texture{ "glActiveTexture" };
	inline c_opengl_export<void(float value, bool invert)> sample_coverage{ "glSampleCoverage" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t internalformat, int width, int height, int depth, int border, int image_size, const void* data)> compressed_tex_image3d{ "glCompressedTexImage3D" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t internalformat, int width, int height, int border, int image_size, const void* data)> compressed_tex_image2d{ "glCompressedTexImage2D" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t internalformat, int width, int border, int image_size, const void* data)> compressed_tex_image1d{ "glCompressedTexImage1D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, std::uint32_t format, int image_size, const void* data)> compressed_tex_sub_image3d{ "glCompressedTexSubImage3D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int yoffset, int width, int height, std::uint32_t format, int image_size, const void* data)> compressed_tex_sub_image2d{ "glCompressedTexSubImage2D" };
	inline c_opengl_export<void(std::uint32_t target, int level, int xoffset, int width, std::uint32_t format, int image_size, const void* data)> compressed_tex_sub_image1d{ "glCompressedTexSubImage1D" };
	inline c_opengl_export<void(std::uint32_t target, int level, void* img)> get_compressed_tex_image{ "glGetCompressedTexImage" };
	inline c_opengl_export<void(std::uint32_t sfactor_rgb, std::uint32_t dfactor_rgb, std::uint32_t sfactor_alpha, std::uint32_t dfactor_alpha)> blend_func_separate{ "glBlendFuncSeparate" };
	inline c_opengl_export<void(std::uint32_t mode, const int* first, const int* count, int drawcount)> multi_draw_arrays{ "glMultiDrawArrays" };
	inline c_opengl_export<void(std::uint32_t mode, const int* count, std::uint32_t type, const void* const* indices, int drawcount)> multi_draw_elements{ "glMultiDrawElements" };
	inline c_opengl_export<void(std::uint32_t pname, float param)> point_parameterf{ "glPointParameterf" };
	inline c_opengl_export<void(std::uint32_t pname, const float* params)> point_parameterfv{ "glPointParameterfv" };
	inline c_opengl_export<void(std::uint32_t pname, int param)> point_parameteri{ "glPointParameteri" };
	inline c_opengl_export<void(std::uint32_t pname, const int* params)> point_parameteriv{ "glPointParameteriv" };
	inline c_opengl_export<void(float red, float green, float blue, float alpha)> blend_color{ "glBlendColor" };
	inline c_opengl_export<void(std::uint32_t mode)> blend_equation{ "glBlendEquation" };
	inline c_opengl_export<void(int n, std::uint32_t* ids)> gen_queries{ "glGenQueries" };
	inline c_opengl_export<void(int n, const std::uint32_t* ids)> delete_queries{ "glDeleteQueries" };
	inline c_opengl_export<bool(std::uint32_t id)> is_query{ "glIsQuery" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t id)> begin_query{ "glBeginQuery" };
	inline c_opengl_export<void(std::uint32_t target)> end_query{ "glEndQuery" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int* params)> get_queryiv{ "glGetQueryiv" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t pname, int* params)> get_query_objectiv{ "glGetQueryObjectiv" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t pname, std::uint32_t* params)> get_query_objectuiv{ "glGetQueryObjectuiv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t buffer)> bind_buffer{ "glBindBuffer" };
	inline c_opengl_export<void(int n, const std::uint32_t* buffers)> delete_buffers{ "glDeleteBuffers" };
	inline c_opengl_export<void(int n, std::uint32_t* buffers)> gen_buffers{ "glGenBuffers" };
	inline c_opengl_export<bool(std::uint32_t buffer)> is_buffer{ "glIsBuffer" };
	inline c_opengl_export<void(std::uint32_t target, std::intptr_t size, const void* data, std::uint32_t usage)> buffer_data{ "glBufferData" };
	inline c_opengl_export<void(std::uint32_t target, std::intptr_t offset, std::intptr_t size, const void* data)> buffer_sub_data{ "glBufferSubData" };
	inline c_opengl_export<void(std::uint32_t target, std::intptr_t offset, std::intptr_t size, void* data)> get_buffer_sub_data{ "glGetBufferSubData" };
	inline c_opengl_export<void*(std::uint32_t target, std::uint32_t access)> map_buffer{ "glMapBuffer" };
	inline c_opengl_export<bool(std::uint32_t target)> unmap_buffer{ "glUnmapBuffer" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int* params)> get_buffer_parameteriv{ "glGetBufferParameteriv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, void** params)> get_buffer_pointerv{ "glGetBufferPointerv" };
	inline c_opengl_export<void(std::uint32_t mode_rgb, std::uint32_t mode_alpha)> blend_equation_separate{ "glBlendEquationSeparate" };
	inline c_opengl_export<void(int n, const std::uint32_t* bufs)> draw_buffers{ "glDrawBuffers" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t sfail, std::uint32_t dpfail, std::uint32_t dppass)> stencil_op_separate{ "glStencilOpSeparate" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t func, int ref, std::uint32_t mask)> stencil_func_separate{ "glStencilFuncSeparate" };
	inline c_opengl_export<void(std::uint32_t face, std::uint32_t mask)> stencil_mask_separate{ "glStencilMaskSeparate" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t shader)> attach_shader{ "glAttachShader" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t index, const char* name)> bind_attrib_location{ "glBindAttribLocation" };
	inline c_opengl_export<void(std::uint32_t shader)> compile_shader{ "glCompileShader" };
	inline c_opengl_export<std::uint32_t()> create_program{ "glCreateProgram" };
	inline c_opengl_export<std::uint32_t(std::uint32_t type)> create_shader{ "glCreateShader" };
	inline c_opengl_export<void(std::uint32_t program)> delete_program{ "glDeleteProgram" };
	inline c_opengl_export<void(std::uint32_t shader)> delete_shader{ "glDeleteShader" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t shader)> detach_shader{ "glDetachShader" };
	inline c_opengl_export<void(std::uint32_t index)> disable_vertex_attrib_array{ "glDisableVertexAttribArray" };
	inline c_opengl_export<void(std::uint32_t index)> enable_vertex_attrib_array{ "glEnableVertexAttribArray" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t index, int buf_size, int* length, int* size, std::uint32_t* type, char* name)> get_active_attrib{ "glGetActiveAttrib" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t index, int buf_size, int* length, int* size, std::uint32_t* type, char* name)> get_active_uniform{ "glGetActiveUniform" };
	inline c_opengl_export<void(std::uint32_t program, int max_count, int* count, std::uint32_t* shaders)> get_attached_shaders{ "glGetAttachedShaders" };
	inline c_opengl_export<int(std::uint32_t program, const char* name)> get_attrib_location{ "glGetAttribLocation" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t pname, int* params)> get_programiv{ "glGetProgramiv" };
	inline c_opengl_export<void(std::uint32_t program, int buf_size, int* length, char* info_log)> get_program_info_log{ "glGetProgramInfoLog" };
	inline c_opengl_export<void(std::uint32_t shader, std::uint32_t pname, int* params)> get_shaderiv{ "glGetShaderiv" };
	inline c_opengl_export<void(std::uint32_t shader, int buf_size, int* length, char* info_log)> get_shader_info_log{ "glGetShaderInfoLog" };
	inline c_opengl_export<void(std::uint32_t shader, int buf_size, int* length, char* source)> get_shader_source{ "glGetShaderSource" };
	inline c_opengl_export<int(std::uint32_t program, const char* name)> get_uniform_location{ "glGetUniformLocation" };
	inline c_opengl_export<void(std::uint32_t program, int location, float* params)> get_uniformfv{ "glGetUniformfv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int* params)> get_uniformiv{ "glGetUniformiv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t pname, double* params)> get_vertex_attribdv{ "glGetVertexAttribdv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t pname, float* params)> get_vertex_attribfv{ "glGetVertexAttribfv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t pname, int* params)> get_vertex_attribiv{ "glGetVertexAttribiv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t pname, void** pointer)> get_vertex_attrib_pointerv{ "glGetVertexAttribPointerv" };
	inline c_opengl_export<bool(std::uint32_t program)> is_program{ "glIsProgram" };
	inline c_opengl_export<bool(std::uint32_t shader)> is_shader{ "glIsShader" };
	inline c_opengl_export<void(std::uint32_t program)> link_program{ "glLinkProgram" };
	inline c_opengl_export<void(std::uint32_t shader, int count, const char* const* string, const int* length)> shader_source{ "glShaderSource" };
	inline c_opengl_export<void(std::uint32_t program)> use_program{ "glUseProgram" };
	inline c_opengl_export<void(int location, float v0)> uniform1f{ "glUniform1f" };
	inline c_opengl_export<void(int location, float v0, float v1)> uniform2f{ "glUniform2f" };
	inline c_opengl_export<void(int location, float v0, float v1, float v2)> uniform3f{ "glUniform3f" };
	inline c_opengl_export<void(int location, float v0, float v1, float v2, float v3)> uniform4f{ "glUniform4f" };
	inline c_opengl_export<void(int location, int v0)> uniform1i{ "glUniform1i" };
	inline c_opengl_export<void(int location, int v0, int v1)> uniform2i{ "glUniform2i" };
	inline c_opengl_export<void(int location, int v0, int v1, int v2)> uniform3i{ "glUniform3i" };
	inline c_opengl_export<void(int location, int v0, int v1, int v2, int v3)> uniform4i{ "glUniform4i" };
	inline c_opengl_export<void(int location, int count, const float* value)> uniform1fv{ "glUniform1fv" };
	inline c_opengl_export<void(int location, int count, const float* value)> uniform2fv{ "glUniform2fv" };
	inline c_opengl_export<void(int location, int count, const float* value)> uniform3fv{ "glUniform3fv" };
	inline c_opengl_export<void(int location, int count, const float* value)> uniform4fv{ "glUniform4fv" };
	inline c_opengl_export<void(int location, int count, const int* value)> uniform1iv{ "glUniform1iv" };
	inline c_opengl_export<void(int location, int count, const int* value)> uniform2iv{ "glUniform2iv" };
	inline c_opengl_export<void(int location, int count, const int* value)> uniform3iv{ "glUniform3iv" };
	inline c_opengl_export<void(int location, int count, const int* value)> uniform4iv{ "glUniform4iv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix2fv{ "glUniformMatrix2fv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix3fv{ "glUniformMatrix3fv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix4fv{ "glUniformMatrix4fv" };
	inline c_opengl_export<void(std::uint32_t program)> validate_program{ "glValidateProgram" };
	inline c_opengl_export<void(std::uint32_t index, double x)> vertex_attrib1d{ "glVertexAttrib1d" };
	inline c_opengl_export<void(std::uint32_t index, const double* v)> vertex_attrib1dv{ "glVertexAttrib1dv" };
	inline c_opengl_export<void(std::uint32_t index, float x)> vertex_attrib1f{ "glVertexAttrib1f" };
	inline c_opengl_export<void(std::uint32_t index, const float* v)> vertex_attrib1fv{ "glVertexAttrib1fv" };
	inline c_opengl_export<void(std::uint32_t index, short x)> vertex_attrib1s{ "glVertexAttrib1s" };
	inline c_opengl_export<void(std::uint32_t index, const short* v)> vertex_attrib1sv{ "glVertexAttrib1sv" };
	inline c_opengl_export<void(std::uint32_t index, double x, double y)> vertex_attrib2d{ "glVertexAttrib2d" };
	inline c_opengl_export<void(std::uint32_t index, const double* v)> vertex_attrib2dv{ "glVertexAttrib2dv" };
	inline c_opengl_export<void(std::uint32_t index, float x, float y)> vertex_attrib2f{ "glVertexAttrib2f" };
	inline c_opengl_export<void(std::uint32_t index, const float* v)> vertex_attrib2fv{ "glVertexAttrib2fv" };
	inline c_opengl_export<void(std::uint32_t index, short x, short y)> vertex_attrib2s{ "glVertexAttrib2s" };
	inline c_opengl_export<void(std::uint32_t index, const short* v)> vertex_attrib2sv{ "glVertexAttrib2sv" };
	inline c_opengl_export<void(std::uint32_t index, double x, double y, double z)> vertex_attrib3d{ "glVertexAttrib3d" };
	inline c_opengl_export<void(std::uint32_t index, const double* v)> vertex_attrib3dv{ "glVertexAttrib3dv" };
	inline c_opengl_export<void(std::uint32_t index, float x, float y, float z)> vertex_attrib3f{ "glVertexAttrib3f" };
	inline c_opengl_export<void(std::uint32_t index, const float* v)> vertex_attrib3fv{ "glVertexAttrib3fv" };
	inline c_opengl_export<void(std::uint32_t index, short x, short y, short z)> vertex_attrib3s{ "glVertexAttrib3s" };
	inline c_opengl_export<void(std::uint32_t index, const short* v)> vertex_attrib3sv{ "glVertexAttrib3sv" };
	inline c_opengl_export<void(std::uint32_t index, const std::int8_t* v)> vertex_attrib4_nbv{ "glVertexAttrib4Nbv" };
	inline c_opengl_export<void(std::uint32_t index, const int* v)> vertex_attrib4_niv{ "glVertexAttrib4Niv" };
	inline c_opengl_export<void(std::uint32_t index, const short* v)> vertex_attrib4_nsv{ "glVertexAttrib4Nsv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint8_t x, std::uint8_t y, std::uint8_t z, std::uint8_t w)> vertex_attrib4_nub{ "glVertexAttrib4Nub" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint8_t* v)> vertex_attrib4_nubv{ "glVertexAttrib4Nubv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint32_t* v)> vertex_attrib4_nuiv{ "glVertexAttrib4Nuiv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint16_t* v)> vertex_attrib4_nusv{ "glVertexAttrib4Nusv" };
	inline c_opengl_export<void(std::uint32_t index, const std::int8_t* v)> vertex_attrib4bv{ "glVertexAttrib4bv" };
	inline c_opengl_export<void(std::uint32_t index, double x, double y, double z, double w)> vertex_attrib4d{ "glVertexAttrib4d" };
	inline c_opengl_export<void(std::uint32_t index, const double* v)> vertex_attrib4dv{ "glVertexAttrib4dv" };
	inline c_opengl_export<void(std::uint32_t index, float x, float y, float z, float w)> vertex_attrib4f{ "glVertexAttrib4f" };
	inline c_opengl_export<void(std::uint32_t index, const float* v)> vertex_attrib4fv{ "glVertexAttrib4fv" };
	inline c_opengl_export<void(std::uint32_t index, const int* v)> vertex_attrib4iv{ "glVertexAttrib4iv" };
	inline c_opengl_export<void(std::uint32_t index, short x, short y, short z, short w)> vertex_attrib4s{ "glVertexAttrib4s" };
	inline c_opengl_export<void(std::uint32_t index, const short* v)> vertex_attrib4sv{ "glVertexAttrib4sv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint8_t* v)> vertex_attrib4ubv{ "glVertexAttrib4ubv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint32_t* v)> vertex_attrib4uiv{ "glVertexAttrib4uiv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint16_t* v)> vertex_attrib4usv{ "glVertexAttrib4usv" };
	inline c_opengl_export<void(std::uint32_t index, int size, std::uint32_t type, bool normalized, int stride, const void* pointer)> vertex_attrib_pointer{ "glVertexAttribPointer" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix2x3fv{ "glUniformMatrix2x3fv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix3x2fv{ "glUniformMatrix3x2fv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix2x4fv{ "glUniformMatrix2x4fv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix4x2fv{ "glUniformMatrix4x2fv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix3x4fv{ "glUniformMatrix3x4fv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const float* value)> uniform_matrix4x3fv{ "glUniformMatrix4x3fv" };
	inline c_opengl_export<void(std::uint32_t index, bool r, bool g, bool b, bool a)> color_maski{ "glColorMaski" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, bool* data)> get_booleani_v{ "glGetBooleani_v" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, int* data)> get_integeri_v{ "glGetIntegeri_v" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index)> enablei{ "glEnablei" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index)> disablei{ "glDisablei" };
	inline c_opengl_export<bool(std::uint32_t target, std::uint32_t index)> is_enabledi{ "glIsEnabledi" };
	inline c_opengl_export<void(std::uint32_t primitive_mode)> begin_transform_feedback{ "glBeginTransformFeedback" };
	inline c_opengl_export<void()> end_transform_feedback{ "glEndTransformFeedback" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, std::uint32_t buffer, std::intptr_t offset, std::intptr_t size)> bind_buffer_range{ "glBindBufferRange" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, std::uint32_t buffer)> bind_buffer_base{ "glBindBufferBase" };
	inline c_opengl_export<void(std::uint32_t program, int count, const char* const* varyings, std::uint32_t buffer_mode)> transform_feedback_varyings{ "glTransformFeedbackVaryings" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t index, int buf_size, int* length, int* size, std::uint32_t* type, char* name)> get_transform_feedback_varying{ "glGetTransformFeedbackVarying" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t clamp)> clamp_color{ "glClampColor" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t mode)> begin_conditional_render{ "glBeginConditionalRender" };
	inline c_opengl_export<void()> end_conditional_render{ "glEndConditionalRender" };
	inline c_opengl_export<void(std::uint32_t index, int size, std::uint32_t type, int stride, const void* pointer)> vertex_attrib_i_pointer{ "glVertexAttribIPointer" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t pname, int* params)> get_vertex_attrib_iiv{ "glGetVertexAttribIiv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t pname, std::uint32_t* params)> get_vertex_attrib_iuiv{ "glGetVertexAttribIuiv" };
	inline c_opengl_export<void(std::uint32_t index, int x)> vertex_attrib_i1i{ "glVertexAttribI1i" };
	inline c_opengl_export<void(std::uint32_t index, int x, int y)> vertex_attrib_i2i{ "glVertexAttribI2i" };
	inline c_opengl_export<void(std::uint32_t index, int x, int y, int z)> vertex_attrib_i3i{ "glVertexAttribI3i" };
	inline c_opengl_export<void(std::uint32_t index, int x, int y, int z, int w)> vertex_attrib_i4i{ "glVertexAttribI4i" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t x)> vertex_attrib_i1ui{ "glVertexAttribI1ui" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t x, std::uint32_t y)> vertex_attrib_i2ui{ "glVertexAttribI2ui" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t x, std::uint32_t y, std::uint32_t z)> vertex_attrib_i3ui{ "glVertexAttribI3ui" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t x, std::uint32_t y, std::uint32_t z, std::uint32_t w)> vertex_attrib_i4ui{ "glVertexAttribI4ui" };
	inline c_opengl_export<void(std::uint32_t index, const int* v)> vertex_attrib_i1iv{ "glVertexAttribI1iv" };
	inline c_opengl_export<void(std::uint32_t index, const int* v)> vertex_attrib_i2iv{ "glVertexAttribI2iv" };
	inline c_opengl_export<void(std::uint32_t index, const int* v)> vertex_attrib_i3iv{ "glVertexAttribI3iv" };
	inline c_opengl_export<void(std::uint32_t index, const int* v)> vertex_attrib_i4iv{ "glVertexAttribI4iv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint32_t* v)> vertex_attrib_i1uiv{ "glVertexAttribI1uiv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint32_t* v)> vertex_attrib_i2uiv{ "glVertexAttribI2uiv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint32_t* v)> vertex_attrib_i3uiv{ "glVertexAttribI3uiv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint32_t* v)> vertex_attrib_i4uiv{ "glVertexAttribI4uiv" };
	inline c_opengl_export<void(std::uint32_t index, const std::int8_t* v)> vertex_attrib_i4bv{ "glVertexAttribI4bv" };
	inline c_opengl_export<void(std::uint32_t index, const short* v)> vertex_attrib_i4sv{ "glVertexAttribI4sv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint8_t* v)> vertex_attrib_i4ubv{ "glVertexAttribI4ubv" };
	inline c_opengl_export<void(std::uint32_t index, const std::uint16_t* v)> vertex_attrib_i4usv{ "glVertexAttribI4usv" };
	inline c_opengl_export<void(std::uint32_t program, int location, std::uint32_t* params)> get_uniformuiv{ "glGetUniformuiv" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t color, const char* name)> bind_frag_data_location{ "glBindFragDataLocation" };
	inline c_opengl_export<int(std::uint32_t program, const char* name)> get_frag_data_location{ "glGetFragDataLocation" };
	inline c_opengl_export<void(int location, std::uint32_t v0)> uniform1ui{ "glUniform1ui" };
	inline c_opengl_export<void(int location, std::uint32_t v0, std::uint32_t v1)> uniform2ui{ "glUniform2ui" };
	inline c_opengl_export<void(int location, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2)> uniform3ui{ "glUniform3ui" };
	inline c_opengl_export<void(int location, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2, std::uint32_t v3)> uniform4ui{ "glUniform4ui" };
	inline c_opengl_export<void(int location, int count, const std::uint32_t* value)> uniform1uiv{ "glUniform1uiv" };
	inline c_opengl_export<void(int location, int count, const std::uint32_t* value)> uniform2uiv{ "glUniform2uiv" };
	inline c_opengl_export<void(int location, int count, const std::uint32_t* value)> uniform3uiv{ "glUniform3uiv" };
	inline c_opengl_export<void(int location, int count, const std::uint32_t* value)> uniform4uiv{ "glUniform4uiv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, const int* params)> tex_parameter_iiv{ "glTexParameterIiv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, const std::uint32_t* params)> tex_parameter_iuiv{ "glTexParameterIuiv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int* params)> get_tex_parameter_iiv{ "glGetTexParameterIiv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, std::uint32_t* params)> get_tex_parameter_iuiv{ "glGetTexParameterIuiv" };
	inline c_opengl_export<void(std::uint32_t buffer, int drawbuffer, const int* value)> clear_bufferiv{ "glClearBufferiv" };
	inline c_opengl_export<void(std::uint32_t buffer, int drawbuffer, const std::uint32_t* value)> clear_bufferuiv{ "glClearBufferuiv" };
	inline c_opengl_export<void(std::uint32_t buffer, int drawbuffer, const float* value)> clear_bufferfv{ "glClearBufferfv" };
	inline c_opengl_export<void(std::uint32_t buffer, int drawbuffer, float depth, int stencil)> clear_bufferfi{ "glClearBufferfi" };
	inline c_opengl_export<bool(std::uint32_t renderbuffer)> is_renderbuffer{ "glIsRenderbuffer" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t renderbuffer)> bind_renderbuffer{ "glBindRenderbuffer" };
	inline c_opengl_export<void(int n, const std::uint32_t* renderbuffers)> delete_renderbuffers{ "glDeleteRenderbuffers" };
	inline c_opengl_export<void(int n, std::uint32_t* renderbuffers)> gen_renderbuffers{ "glGenRenderbuffers" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t internalformat, int width, int height)> renderbuffer_storage{ "glRenderbufferStorage" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int* params)> get_renderbuffer_parameteriv{ "glGetRenderbufferParameteriv" };
	inline c_opengl_export<bool(std::uint32_t framebuffer)> is_framebuffer{ "glIsFramebuffer" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t framebuffer)> bind_framebuffer{ "glBindFramebuffer" };
	inline c_opengl_export<void(int n, const std::uint32_t* framebuffers)> delete_framebuffers{ "glDeleteFramebuffers" };
	inline c_opengl_export<void(int n, std::uint32_t* framebuffers)> gen_framebuffers{ "glGenFramebuffers" };
	inline c_opengl_export<std::uint32_t(std::uint32_t target)> check_framebuffer_status{ "glCheckFramebufferStatus" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t attachment, std::uint32_t textarget, std::uint32_t texture, int level)> framebuffer_texture1d{ "glFramebufferTexture1D" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t attachment, std::uint32_t textarget, std::uint32_t texture, int level)> framebuffer_texture2d{ "glFramebufferTexture2D" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t attachment, std::uint32_t textarget, std::uint32_t texture, int level, int zoffset)> framebuffer_texture3d{ "glFramebufferTexture3D" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t attachment, std::uint32_t renderbuffertarget, std::uint32_t renderbuffer)> framebuffer_renderbuffer{ "glFramebufferRenderbuffer" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t attachment, std::uint32_t pname, int* params)> get_framebuffer_attachment_parameteriv{ "glGetFramebufferAttachmentParameteriv" };
	inline c_opengl_export<void(std::uint32_t target)> generate_mipmap{ "glGenerateMipmap" };
	inline c_opengl_export<void(int src_x0, int src_y0, int src_x1, int src_y1, int dst_x0, int dst_y0, int dst_x1, int dst_y1, std::uint32_t mask, std::uint32_t filter)> blit_framebuffer{ "glBlitFramebuffer" };
	inline c_opengl_export<void(std::uint32_t target, int samples, std::uint32_t internalformat, int width, int height)> renderbuffer_storage_multisample{ "glRenderbufferStorageMultisample" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t attachment, std::uint32_t texture, int level, int layer)> framebuffer_texture_layer{ "glFramebufferTextureLayer" };
	inline c_opengl_export<void*(std::uint32_t target, std::intptr_t offset, std::intptr_t length, std::uint32_t access)> map_buffer_range{ "glMapBufferRange" };
	inline c_opengl_export<void(std::uint32_t target, std::intptr_t offset, std::intptr_t length)> flush_mapped_buffer_range{ "glFlushMappedBufferRange" };
	inline c_opengl_export<void(std::uint32_t array)> bind_vertex_array{ "glBindVertexArray" };
	inline c_opengl_export<void(int n, const std::uint32_t* arrays)> delete_vertex_arrays{ "glDeleteVertexArrays" };
	inline c_opengl_export<void(int n, std::uint32_t* arrays)> gen_vertex_arrays{ "glGenVertexArrays" };
	inline c_opengl_export<bool(std::uint32_t array)> is_vertex_array{ "glIsVertexArray" };
	inline c_opengl_export<void(std::uint32_t mode, int first, int count, int instancecount)> draw_arrays_instanced{ "glDrawArraysInstanced" };
	inline c_opengl_export<void(std::uint32_t mode, int count, std::uint32_t type, const void* indices, int instancecount)> draw_elements_instanced{ "glDrawElementsInstanced" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t internalformat, std::uint32_t buffer)> tex_buffer{ "glTexBuffer" };
	inline c_opengl_export<void(std::uint32_t index)> primitive_restart_index{ "glPrimitiveRestartIndex" };
	inline c_opengl_export<void(std::uint32_t read_target, std::uint32_t write_target, std::intptr_t read_offset, std::intptr_t write_offset, std::intptr_t size)> copy_buffer_sub_data{ "glCopyBufferSubData" };
	inline c_opengl_export<void(std::uint32_t program, int uniform_count, const char* const* uniform_names, std::uint32_t* uniform_indices)> get_uniform_indices{ "glGetUniformIndices" };
	inline c_opengl_export<void(std::uint32_t program, int uniform_count, const std::uint32_t* uniform_indices, std::uint32_t pname, int* params)> get_active_uniformsiv{ "glGetActiveUniformsiv" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t uniform_index, int buf_size, int* length, char* uniform_name)> get_active_uniform_name{ "glGetActiveUniformName" };
	inline c_opengl_export<std::uint32_t(std::uint32_t program, const char* uniform_block_name)> get_uniform_block_index{ "glGetUniformBlockIndex" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t uniform_block_index, std::uint32_t pname, int* params)> get_active_uniform_blockiv{ "glGetActiveUniformBlockiv" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t uniform_block_index, int buf_size, int* length, char* uniform_block_name)> get_active_uniform_block_name{ "glGetActiveUniformBlockName" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t uniform_block_index, std::uint32_t uniform_block_binding)> uniform_block_binding{ "glUniformBlockBinding" };
	inline c_opengl_export<void(std::uint32_t mode, int count, std::uint32_t type, const void* indices, int basevertex)> draw_elements_base_vertex{ "glDrawElementsBaseVertex" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t start, std::uint32_t end, int count, std::uint32_t type, const void* indices, int basevertex)> draw_range_elements_base_vertex{ "glDrawRangeElementsBaseVertex" };
	inline c_opengl_export<void(std::uint32_t mode, int count, std::uint32_t type, const void* indices, int instancecount, int basevertex)> draw_elements_instanced_base_vertex{ "glDrawElementsInstancedBaseVertex" };
	inline c_opengl_export<void(std::uint32_t mode, const int* count, std::uint32_t type, const void* const* indices, int drawcount, const int* basevertex)> multi_draw_elements_base_vertex{ "glMultiDrawElementsBaseVertex" };
	inline c_opengl_export<void(std::uint32_t mode)> provoking_vertex{ "glProvokingVertex" };
	inline c_opengl_export<void(std::uint32_t pname, std::int64_t* data)> get_integer64v{ "glGetInteger64v" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, std::int64_t* data)> get_integer64i_v{ "glGetInteger64i_v" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, std::int64_t* params)> get_buffer_parameteri64v{ "glGetBufferParameteri64v" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t attachment, std::uint32_t texture, int level)> framebuffer_texture{ "glFramebufferTexture" };
	inline c_opengl_export<void(std::uint32_t target, int samples, std::uint32_t internalformat, int width, int height, bool fixedsamplelocations)> tex_image2d_multisample{ "glTexImage2DMultisample" };
	inline c_opengl_export<void(std::uint32_t target, int samples, std::uint32_t internalformat, int width, int height, int depth, bool fixedsamplelocations)> tex_image3d_multisample{ "glTexImage3DMultisample" };
	inline c_opengl_export<void(std::uint32_t pname, std::uint32_t index, float* val)> get_multisamplefv{ "glGetMultisamplefv" };
	inline c_opengl_export<void(std::uint32_t mask_number, std::uint32_t mask)> sample_maski{ "glSampleMaski" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t color_number, std::uint32_t index, const char* name)> bind_frag_data_location_indexed{ "glBindFragDataLocationIndexed" };
	inline c_opengl_export<int(std::uint32_t program, const char* name)> get_frag_data_index{ "glGetFragDataIndex" };
	inline c_opengl_export<void(int count, std::uint32_t* samplers)> gen_samplers{ "glGenSamplers" };
	inline c_opengl_export<void(int count, const std::uint32_t* samplers)> delete_samplers{ "glDeleteSamplers" };
	inline c_opengl_export<bool(std::uint32_t sampler)> is_sampler{ "glIsSampler" };
	inline c_opengl_export<void(std::uint32_t unit, std::uint32_t sampler)> bind_sampler{ "glBindSampler" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, int param)> sampler_parameteri{ "glSamplerParameteri" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, const int* param)> sampler_parameteriv{ "glSamplerParameteriv" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, float param)> sampler_parameterf{ "glSamplerParameterf" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, const float* param)> sampler_parameterfv{ "glSamplerParameterfv" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, const int* param)> sampler_parameter_iiv{ "glSamplerParameterIiv" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, const std::uint32_t* param)> sampler_parameter_iuiv{ "glSamplerParameterIuiv" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, int* params)> get_sampler_parameteriv{ "glGetSamplerParameteriv" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, int* params)> get_sampler_parameter_iiv{ "glGetSamplerParameterIiv" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, float* params)> get_sampler_parameterfv{ "glGetSamplerParameterfv" };
	inline c_opengl_export<void(std::uint32_t sampler, std::uint32_t pname, std::uint32_t* params)> get_sampler_parameter_iuiv{ "glGetSamplerParameterIuiv" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t target)> query_counter{ "glQueryCounter" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t pname, std::int64_t* params)> get_query_objecti64v{ "glGetQueryObjecti64v" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t pname, std::uint64_t* params)> get_query_objectui64v{ "glGetQueryObjectui64v" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t divisor)> vertex_attrib_divisor{ "glVertexAttribDivisor" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t type, bool normalized, std::uint32_t value)> vertex_attrib_p1ui{ "glVertexAttribP1ui" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t type, bool normalized, const std::uint32_t* value)> vertex_attrib_p1uiv{ "glVertexAttribP1uiv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t type, bool normalized, std::uint32_t value)> vertex_attrib_p2ui{ "glVertexAttribP2ui" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t type, bool normalized, const std::uint32_t* value)> vertex_attrib_p2uiv{ "glVertexAttribP2uiv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t type, bool normalized, std::uint32_t value)> vertex_attrib_p3ui{ "glVertexAttribP3ui" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t type, bool normalized, const std::uint32_t* value)> vertex_attrib_p3uiv{ "glVertexAttribP3uiv" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t type, bool normalized, std::uint32_t value)> vertex_attrib_p4ui{ "glVertexAttribP4ui" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t type, bool normalized, const std::uint32_t* value)> vertex_attrib_p4uiv{ "glVertexAttribP4uiv" };
	inline c_opengl_export<void(float value)> min_sample_shading{ "glMinSampleShading" };
	inline c_opengl_export<void(std::uint32_t buf, std::uint32_t mode)> blend_equationi{ "glBlendEquationi" };
	inline c_opengl_export<void(std::uint32_t buf, std::uint32_t mode_rgb, std::uint32_t mode_alpha)> blend_equation_separatei{ "glBlendEquationSeparatei" };
	inline c_opengl_export<void(std::uint32_t buf, std::uint32_t src, std::uint32_t dst)> blend_funci{ "glBlendFunci" };
	inline c_opengl_export<void(std::uint32_t buf, std::uint32_t src_rgb, std::uint32_t dst_rgb, std::uint32_t src_alpha, std::uint32_t dst_alpha)> blend_func_separatei{ "glBlendFuncSeparatei" };
	inline c_opengl_export<void(std::uint32_t mode, const void* indirect)> draw_arrays_indirect{ "glDrawArraysIndirect" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t type, const void* indirect)> draw_elements_indirect{ "glDrawElementsIndirect" };
	inline c_opengl_export<void(int location, double x)> uniform1d{ "glUniform1d" };
	inline c_opengl_export<void(int location, double x, double y)> uniform2d{ "glUniform2d" };
	inline c_opengl_export<void(int location, double x, double y, double z)> uniform3d{ "glUniform3d" };
	inline c_opengl_export<void(int location, double x, double y, double z, double w)> uniform4d{ "glUniform4d" };
	inline c_opengl_export<void(int location, int count, const double* value)> uniform1dv{ "glUniform1dv" };
	inline c_opengl_export<void(int location, int count, const double* value)> uniform2dv{ "glUniform2dv" };
	inline c_opengl_export<void(int location, int count, const double* value)> uniform3dv{ "glUniform3dv" };
	inline c_opengl_export<void(int location, int count, const double* value)> uniform4dv{ "glUniform4dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix2dv{ "glUniformMatrix2dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix3dv{ "glUniformMatrix3dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix4dv{ "glUniformMatrix4dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix2x3dv{ "glUniformMatrix2x3dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix2x4dv{ "glUniformMatrix2x4dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix3x2dv{ "glUniformMatrix3x2dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix3x4dv{ "glUniformMatrix3x4dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix4x2dv{ "glUniformMatrix4x2dv" };
	inline c_opengl_export<void(int location, int count, bool transpose, const double* value)> uniform_matrix4x3dv{ "glUniformMatrix4x3dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, double* params)> get_uniformdv{ "glGetUniformdv" };
	inline c_opengl_export<int(std::uint32_t program, std::uint32_t shadertype, const char* name)> get_subroutine_uniform_location{ "glGetSubroutineUniformLocation" };
	inline c_opengl_export<std::uint32_t(std::uint32_t program, std::uint32_t shadertype, const char* name)> get_subroutine_index{ "glGetSubroutineIndex" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t shadertype, std::uint32_t index, std::uint32_t pname, int* values)> get_active_subroutine_uniformiv{ "glGetActiveSubroutineUniformiv" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t shadertype, std::uint32_t index, int buf_size, int* length, char* name)> get_active_subroutine_uniform_name{ "glGetActiveSubroutineUniformName" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t shadertype, std::uint32_t index, int buf_size, int* length, char* name)> get_active_subroutine_name{ "glGetActiveSubroutineName" };
	inline c_opengl_export<void(std::uint32_t shadertype, int count, const std::uint32_t* indices)> uniform_subroutinesuiv{ "glUniformSubroutinesuiv" };
	inline c_opengl_export<void(std::uint32_t shadertype, int location, std::uint32_t* params)> get_uniform_subroutineuiv{ "glGetUniformSubroutineuiv" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t shadertype, std::uint32_t pname, int* values)> get_program_stageiv{ "glGetProgramStageiv" };
	inline c_opengl_export<void(std::uint32_t pname, int value)> patch_parameteri{ "glPatchParameteri" };
	inline c_opengl_export<void(std::uint32_t pname, const float* values)> patch_parameterfv{ "glPatchParameterfv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t id)> bind_transform_feedback{ "glBindTransformFeedback" };
	inline c_opengl_export<void(int n, const std::uint32_t* ids)> delete_transform_feedbacks{ "glDeleteTransformFeedbacks" };
	inline c_opengl_export<void(int n, std::uint32_t* ids)> gen_transform_feedbacks{ "glGenTransformFeedbacks" };
	inline c_opengl_export<bool(std::uint32_t id)> is_transform_feedback{ "glIsTransformFeedback" };
	inline c_opengl_export<void()> pause_transform_feedback{ "glPauseTransformFeedback" };
	inline c_opengl_export<void()> resume_transform_feedback{ "glResumeTransformFeedback" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t id)> draw_transform_feedback{ "glDrawTransformFeedback" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t id, std::uint32_t stream)> draw_transform_feedback_stream{ "glDrawTransformFeedbackStream" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, std::uint32_t id)> begin_query_indexed{ "glBeginQueryIndexed" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index)> end_query_indexed{ "glEndQueryIndexed" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, std::uint32_t pname, int* params)> get_query_indexediv{ "glGetQueryIndexediv" };
	inline c_opengl_export<void()> release_shader_compiler{ "glReleaseShaderCompiler" };
	inline c_opengl_export<void(int count, const std::uint32_t* shaders, std::uint32_t binary_format, const void* binary, int length)> shader_binary{ "glShaderBinary" };
	inline c_opengl_export<void(std::uint32_t shadertype, std::uint32_t precisiontype, int* range, int* precision)> get_shader_precision_format{ "glGetShaderPrecisionFormat" };
	inline c_opengl_export<void(float n, float f)> depth_rangef{ "glDepthRangef" };
	inline c_opengl_export<void(float d)> clear_depthf{ "glClearDepthf" };
	inline c_opengl_export<void(std::uint32_t program, int buf_size, int* length, std::uint32_t* binary_format, void* binary)> get_program_binary{ "glGetProgramBinary" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t binary_format, const void* binary, int length)> program_binary{ "glProgramBinary" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t pname, int value)> program_parameteri{ "glProgramParameteri" };
	inline c_opengl_export<void(std::uint32_t pipeline, std::uint32_t stages, std::uint32_t program)> use_program_stages{ "glUseProgramStages" };
	inline c_opengl_export<void(std::uint32_t pipeline, std::uint32_t program)> active_shader_program{ "glActiveShaderProgram" };
	inline c_opengl_export<std::uint32_t(std::uint32_t type, int count, const char* const* strings)> create_shader_programv{ "glCreateShaderProgramv" };
	inline c_opengl_export<void(std::uint32_t pipeline)> bind_program_pipeline{ "glBindProgramPipeline" };
	inline c_opengl_export<void(int n, const std::uint32_t* pipelines)> delete_program_pipelines{ "glDeleteProgramPipelines" };
	inline c_opengl_export<void(int n, std::uint32_t* pipelines)> gen_program_pipelines{ "glGenProgramPipelines" };
	inline c_opengl_export<bool(std::uint32_t pipeline)> is_program_pipeline{ "glIsProgramPipeline" };
	inline c_opengl_export<void(std::uint32_t pipeline, std::uint32_t pname, int* params)> get_program_pipelineiv{ "glGetProgramPipelineiv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int v0)> program_uniform1i{ "glProgramUniform1i" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const int* value)> program_uniform1iv{ "glProgramUniform1iv" };
	inline c_opengl_export<void(std::uint32_t program, int location, float v0)> program_uniform1f{ "glProgramUniform1f" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const float* value)> program_uniform1fv{ "glProgramUniform1fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, double v0)> program_uniform1d{ "glProgramUniform1d" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const double* value)> program_uniform1dv{ "glProgramUniform1dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, std::uint32_t v0)> program_uniform1ui{ "glProgramUniform1ui" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const std::uint32_t* value)> program_uniform1uiv{ "glProgramUniform1uiv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int v0, int v1)> program_uniform2i{ "glProgramUniform2i" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const int* value)> program_uniform2iv{ "glProgramUniform2iv" };
	inline c_opengl_export<void(std::uint32_t program, int location, float v0, float v1)> program_uniform2f{ "glProgramUniform2f" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const float* value)> program_uniform2fv{ "glProgramUniform2fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, double v0, double v1)> program_uniform2d{ "glProgramUniform2d" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const double* value)> program_uniform2dv{ "glProgramUniform2dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, std::uint32_t v0, std::uint32_t v1)> program_uniform2ui{ "glProgramUniform2ui" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const std::uint32_t* value)> program_uniform2uiv{ "glProgramUniform2uiv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int v0, int v1, int v2)> program_uniform3i{ "glProgramUniform3i" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const int* value)> program_uniform3iv{ "glProgramUniform3iv" };
	inline c_opengl_export<void(std::uint32_t program, int location, float v0, float v1, float v2)> program_uniform3f{ "glProgramUniform3f" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const float* value)> program_uniform3fv{ "glProgramUniform3fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, double v0, double v1, double v2)> program_uniform3d{ "glProgramUniform3d" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const double* value)> program_uniform3dv{ "glProgramUniform3dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2)> program_uniform3ui{ "glProgramUniform3ui" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const std::uint32_t* value)> program_uniform3uiv{ "glProgramUniform3uiv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int v0, int v1, int v2, int v3)> program_uniform4i{ "glProgramUniform4i" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const int* value)> program_uniform4iv{ "glProgramUniform4iv" };
	inline c_opengl_export<void(std::uint32_t program, int location, float v0, float v1, float v2, float v3)> program_uniform4f{ "glProgramUniform4f" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const float* value)> program_uniform4fv{ "glProgramUniform4fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, double v0, double v1, double v2, double v3)> program_uniform4d{ "glProgramUniform4d" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const double* value)> program_uniform4dv{ "glProgramUniform4dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, std::uint32_t v0, std::uint32_t v1, std::uint32_t v2, std::uint32_t v3)> program_uniform4ui{ "glProgramUniform4ui" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, const std::uint32_t* value)> program_uniform4uiv{ "glProgramUniform4uiv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix2fv{ "glProgramUniformMatrix2fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix3fv{ "glProgramUniformMatrix3fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix4fv{ "glProgramUniformMatrix4fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix2dv{ "glProgramUniformMatrix2dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix3dv{ "glProgramUniformMatrix3dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix4dv{ "glProgramUniformMatrix4dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix2x3fv{ "glProgramUniformMatrix2x3fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix3x2fv{ "glProgramUniformMatrix3x2fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix2x4fv{ "glProgramUniformMatrix2x4fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix4x2fv{ "glProgramUniformMatrix4x2fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix3x4fv{ "glProgramUniformMatrix3x4fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const float* value)> program_uniform_matrix4x3fv{ "glProgramUniformMatrix4x3fv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix2x3dv{ "glProgramUniformMatrix2x3dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix3x2dv{ "glProgramUniformMatrix3x2dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix2x4dv{ "glProgramUniformMatrix2x4dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix4x2dv{ "glProgramUniformMatrix4x2dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix3x4dv{ "glProgramUniformMatrix3x4dv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int count, bool transpose, const double* value)> program_uniform_matrix4x3dv{ "glProgramUniformMatrix4x3dv" };
	inline c_opengl_export<void(std::uint32_t pipeline)> validate_program_pipeline{ "glValidateProgramPipeline" };
	inline c_opengl_export<void(std::uint32_t pipeline, int buf_size, int* length, char* info_log)> get_program_pipeline_info_log{ "glGetProgramPipelineInfoLog" };
	inline c_opengl_export<void(std::uint32_t index, double x)> vertex_attrib_l1d{ "glVertexAttribL1d" };
	inline c_opengl_export<void(std::uint32_t index, double x, double y)> vertex_attrib_l2d{ "glVertexAttribL2d" };
	inline c_opengl_export<void(std::uint32_t index, double x, double y, double z)> vertex_attrib_l3d{ "glVertexAttribL3d" };
	inline c_opengl_export<void(std::uint32_t index, double x, double y, double z, double w)> vertex_attrib_l4d{ "glVertexAttribL4d" };
	inline c_opengl_export<void(std::uint32_t index, const double* v)> vertex_attrib_l1dv{ "glVertexAttribL1dv" };
	inline c_opengl_export<void(std::uint32_t index, const double* v)> vertex_attrib_l2dv{ "glVertexAttribL2dv" };
	inline c_opengl_export<void(std::uint32_t index, const double* v)> vertex_attrib_l3dv{ "glVertexAttribL3dv" };
	inline c_opengl_export<void(std::uint32_t index, const double* v)> vertex_attrib_l4dv{ "glVertexAttribL4dv" };
	inline c_opengl_export<void(std::uint32_t index, int size, std::uint32_t type, int stride, const void* pointer)> vertex_attrib_l_pointer{ "glVertexAttribLPointer" };
	inline c_opengl_export<void(std::uint32_t index, std::uint32_t pname, double* params)> get_vertex_attrib_ldv{ "glGetVertexAttribLdv" };
	inline c_opengl_export<void(std::uint32_t first, int count, const float* v)> viewport_arrayv{ "glViewportArrayv" };
	inline c_opengl_export<void(std::uint32_t index, float x, float y, float w, float h)> viewport_indexedf{ "glViewportIndexedf" };
	inline c_opengl_export<void(std::uint32_t index, const float* v)> viewport_indexedfv{ "glViewportIndexedfv" };
	inline c_opengl_export<void(std::uint32_t first, int count, const int* v)> scissor_arrayv{ "glScissorArrayv" };
	inline c_opengl_export<void(std::uint32_t index, int left, int bottom, int width, int height)> scissor_indexed{ "glScissorIndexed" };
	inline c_opengl_export<void(std::uint32_t index, const int* v)> scissor_indexedv{ "glScissorIndexedv" };
	inline c_opengl_export<void(std::uint32_t first, int count, const double* v)> depth_range_arrayv{ "glDepthRangeArrayv" };
	inline c_opengl_export<void(std::uint32_t index, double n, double f)> depth_range_indexed{ "glDepthRangeIndexed" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, float* data)> get_floati_v{ "glGetFloati_v" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t index, double* data)> get_doublei_v{ "glGetDoublei_v" };
	inline c_opengl_export<void(std::uint32_t mode, int first, int count, int instancecount, std::uint32_t baseinstance)> draw_arrays_instanced_base_instance{ "glDrawArraysInstancedBaseInstance" };
	inline c_opengl_export<void(std::uint32_t mode, int count, std::uint32_t type, const void* indices, int instancecount, std::uint32_t baseinstance)> draw_elements_instanced_base_instance{ "glDrawElementsInstancedBaseInstance" };
	inline c_opengl_export<void(std::uint32_t mode, int count, std::uint32_t type, const void* indices, int instancecount, int basevertex, std::uint32_t baseinstance)> draw_elements_instanced_base_vertex_base_instance{ "glDrawElementsInstancedBaseVertexBaseInstance" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t internalformat, std::uint32_t pname, int count, int* params)> get_internalformativ{ "glGetInternalformativ" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t buffer_index, std::uint32_t pname, int* params)> get_active_atomic_counter_bufferiv{ "glGetActiveAtomicCounterBufferiv" };
	inline c_opengl_export<void(std::uint32_t unit, std::uint32_t texture, int level, bool layered, int layer, std::uint32_t access, std::uint32_t format)> bind_image_texture{ "glBindImageTexture" };
	inline c_opengl_export<void(std::uint32_t barriers)> memory_barrier{ "glMemoryBarrier" };
	inline c_opengl_export<void(std::uint32_t target, int levels, std::uint32_t internalformat, int width)> tex_storage1d{ "glTexStorage1D" };
	inline c_opengl_export<void(std::uint32_t target, int levels, std::uint32_t internalformat, int width, int height)> tex_storage2d{ "glTexStorage2D" };
	inline c_opengl_export<void(std::uint32_t target, int levels, std::uint32_t internalformat, int width, int height, int depth)> tex_storage3d{ "glTexStorage3D" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t id, int instancecount)> draw_transform_feedback_instanced{ "glDrawTransformFeedbackInstanced" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t id, std::uint32_t stream, int instancecount)> draw_transform_feedback_stream_instanced{ "glDrawTransformFeedbackStreamInstanced" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t internalformat, std::uint32_t format, std::uint32_t type, const void* data)> clear_buffer_data{ "glClearBufferData" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t internalformat, std::intptr_t offset, std::intptr_t size, std::uint32_t format, std::uint32_t type, const void* data)> clear_buffer_sub_data{ "glClearBufferSubData" };
	inline c_opengl_export<void(std::uint32_t num_groups_x, std::uint32_t num_groups_y, std::uint32_t num_groups_z)> dispatch_compute{ "glDispatchCompute" };
	inline c_opengl_export<void(std::intptr_t indirect)> dispatch_compute_indirect{ "glDispatchComputeIndirect" };
	inline c_opengl_export<void(std::uint32_t src_name, std::uint32_t src_target, int src_level, int src_x, int src_y, int src_z, std::uint32_t dst_name, std::uint32_t dst_target, int dst_level, int dst_x, int dst_y, int dst_z, int src_width, int src_height, int src_depth)> copy_image_sub_data{ "glCopyImageSubData" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int param)> framebuffer_parameteri{ "glFramebufferParameteri" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t pname, int* params)> get_framebuffer_parameteriv{ "glGetFramebufferParameteriv" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t internalformat, std::uint32_t pname, int count, std::int64_t* params)> get_internalformati64v{ "glGetInternalformati64v" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth)> invalidate_tex_sub_image{ "glInvalidateTexSubImage" };
	inline c_opengl_export<void(std::uint32_t texture, int level)> invalidate_tex_image{ "glInvalidateTexImage" };
	inline c_opengl_export<void(std::uint32_t buffer, std::intptr_t offset, std::intptr_t length)> invalidate_buffer_sub_data{ "glInvalidateBufferSubData" };
	inline c_opengl_export<void(std::uint32_t buffer)> invalidate_buffer_data{ "glInvalidateBufferData" };
	inline c_opengl_export<void(std::uint32_t target, int num_attachments, const std::uint32_t* attachments)> invalidate_framebuffer{ "glInvalidateFramebuffer" };
	inline c_opengl_export<void(std::uint32_t target, int num_attachments, const std::uint32_t* attachments, int x, int y, int width, int height)> invalidate_sub_framebuffer{ "glInvalidateSubFramebuffer" };
	inline c_opengl_export<void(std::uint32_t mode, const void* indirect, int drawcount, int stride)> multi_draw_arrays_indirect{ "glMultiDrawArraysIndirect" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t type, const void* indirect, int drawcount, int stride)> multi_draw_elements_indirect{ "glMultiDrawElementsIndirect" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t program_interface, std::uint32_t pname, int* params)> get_program_interfaceiv{ "glGetProgramInterfaceiv" };
	inline c_opengl_export<std::uint32_t(std::uint32_t program, std::uint32_t program_interface, const char* name)> get_program_resource_index{ "glGetProgramResourceIndex" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t program_interface, std::uint32_t index, int buf_size, int* length, char* name)> get_program_resource_name{ "glGetProgramResourceName" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t program_interface, std::uint32_t index, int prop_count, const std::uint32_t* props, int count, int* length, int* params)> get_program_resourceiv{ "glGetProgramResourceiv" };
	inline c_opengl_export<int(std::uint32_t program, std::uint32_t program_interface, const char* name)> get_program_resource_location{ "glGetProgramResourceLocation" };
	inline c_opengl_export<int(std::uint32_t program, std::uint32_t program_interface, const char* name)> get_program_resource_location_index{ "glGetProgramResourceLocationIndex" };
	inline c_opengl_export<void(std::uint32_t program, std::uint32_t storage_block_index, std::uint32_t storage_block_binding)> shader_storage_block_binding{ "glShaderStorageBlockBinding" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t internalformat, std::uint32_t buffer, std::intptr_t offset, std::intptr_t size)> tex_buffer_range{ "glTexBufferRange" };
	inline c_opengl_export<void(std::uint32_t target, int samples, std::uint32_t internalformat, int width, int height, bool fixedsamplelocations)> tex_storage2d_multisample{ "glTexStorage2DMultisample" };
	inline c_opengl_export<void(std::uint32_t target, int samples, std::uint32_t internalformat, int width, int height, int depth, bool fixedsamplelocations)> tex_storage3d_multisample{ "glTexStorage3DMultisample" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t target, std::uint32_t origtexture, std::uint32_t internalformat, std::uint32_t minlevel, std::uint32_t numlevels, std::uint32_t minlayer, std::uint32_t numlayers)> texture_view{ "glTextureView" };
	inline c_opengl_export<void(std::uint32_t bindingindex, std::uint32_t buffer, std::intptr_t offset, int stride)> bind_vertex_buffer{ "glBindVertexBuffer" };
	inline c_opengl_export<void(std::uint32_t attribindex, int size, std::uint32_t type, bool normalized, std::uint32_t relativeoffset)> vertex_attrib_format{ "glVertexAttribFormat" };
	inline c_opengl_export<void(std::uint32_t attribindex, int size, std::uint32_t type, std::uint32_t relativeoffset)> vertex_attrib_i_format{ "glVertexAttribIFormat" };
	inline c_opengl_export<void(std::uint32_t attribindex, int size, std::uint32_t type, std::uint32_t relativeoffset)> vertex_attrib_l_format{ "glVertexAttribLFormat" };
	inline c_opengl_export<void(std::uint32_t attribindex, std::uint32_t bindingindex)> vertex_attrib_binding{ "glVertexAttribBinding" };
	inline c_opengl_export<void(std::uint32_t bindingindex, std::uint32_t divisor)> vertex_binding_divisor{ "glVertexBindingDivisor" };
	inline c_opengl_export<void(std::uint32_t source, std::uint32_t type, std::uint32_t severity, int count, const std::uint32_t* ids, bool enabled)> debug_message_control{ "glDebugMessageControl" };
	inline c_opengl_export<void(std::uint32_t source, std::uint32_t type, std::uint32_t id, std::uint32_t severity, int length, const char* buf)> debug_message_insert{ "glDebugMessageInsert" };
	inline c_opengl_export<void(void(__stdcall *callback)(std::uint32_t source, std::uint32_t type, std::uint32_t id, std::uint32_t severity, int length, const char* message, const void* user_param), const void* user_param)> debug_message_callback{ "glDebugMessageCallback" };
	inline c_opengl_export<std::uint32_t(std::uint32_t count, int buf_size, std::uint32_t* sources, std::uint32_t* types, std::uint32_t* ids, std::uint32_t* severities, int* lengths, char* message_log)> get_debug_message_log{ "glGetDebugMessageLog" };
	inline c_opengl_export<void(std::uint32_t source, std::uint32_t id, int length, const char* message)> push_debug_group{ "glPushDebugGroup" };
	inline c_opengl_export<void()> pop_debug_group{ "glPopDebugGroup" };
	inline c_opengl_export<void(std::uint32_t identifier, std::uint32_t name, int length, const char* label)> object_label{ "glObjectLabel" };
	inline c_opengl_export<void(std::uint32_t identifier, std::uint32_t name, int buf_size, int* length, char* label)> get_object_label{ "glGetObjectLabel" };
	inline c_opengl_export<void(const void* ptr, int length, const char* label)> object_ptr_label{ "glObjectPtrLabel" };
	inline c_opengl_export<void(const void* ptr, int buf_size, int* length, char* label)> get_object_ptr_label{ "glGetObjectPtrLabel" };
	inline c_opengl_export<void(std::uint32_t target, std::intptr_t size, const void* data, std::uint32_t flags)> buffer_storage{ "glBufferStorage" };
	inline c_opengl_export<void(std::uint32_t texture, int level, std::uint32_t format, std::uint32_t type, const void* data)> clear_tex_image{ "glClearTexImage" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, std::uint32_t format, std::uint32_t type, const void* data)> clear_tex_sub_image{ "glClearTexSubImage" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t first, int count, const std::uint32_t* buffers)> bind_buffers_base{ "glBindBuffersBase" };
	inline c_opengl_export<void(std::uint32_t target, std::uint32_t first, int count, const std::uint32_t* buffers, const std::intptr_t* offsets, const std::intptr_t* sizes)> bind_buffers_range{ "glBindBuffersRange" };
	inline c_opengl_export<void(std::uint32_t first, int count, const std::uint32_t* textures)> bind_textures{ "glBindTextures" };
	inline c_opengl_export<void(std::uint32_t first, int count, const std::uint32_t* samplers)> bind_samplers{ "glBindSamplers" };
	inline c_opengl_export<void(std::uint32_t first, int count, const std::uint32_t* textures)> bind_image_textures{ "glBindImageTextures" };
	inline c_opengl_export<void(std::uint32_t first, int count, const std::uint32_t* buffers, const std::intptr_t* offsets, const int* strides)> bind_vertex_buffers{ "glBindVertexBuffers" };
	inline c_opengl_export<void(std::uint32_t origin, std::uint32_t depth)> clip_control{ "glClipControl" };
	inline c_opengl_export<void(int n, std::uint32_t* ids)> create_transform_feedbacks{ "glCreateTransformFeedbacks" };
	inline c_opengl_export<void(std::uint32_t xfb, std::uint32_t index, std::uint32_t buffer)> transform_feedback_buffer_base{ "glTransformFeedbackBufferBase" };
	inline c_opengl_export<void(std::uint32_t xfb, std::uint32_t index, std::uint32_t buffer, std::intptr_t offset, std::intptr_t size)> transform_feedback_buffer_range{ "glTransformFeedbackBufferRange" };
	inline c_opengl_export<void(std::uint32_t xfb, std::uint32_t pname, int* param)> get_transform_feedbackiv{ "glGetTransformFeedbackiv" };
	inline c_opengl_export<void(std::uint32_t xfb, std::uint32_t pname, std::uint32_t index, int* param)> get_transform_feedbacki_v{ "glGetTransformFeedbacki_v" };
	inline c_opengl_export<void(std::uint32_t xfb, std::uint32_t pname, std::uint32_t index, std::int64_t* param)> get_transform_feedbacki64_v{ "glGetTransformFeedbacki64_v" };
	inline c_opengl_export<void(int n, std::uint32_t* buffers)> create_buffers{ "glCreateBuffers" };
	inline c_opengl_export<void(std::uint32_t buffer, std::intptr_t size, const void* data, std::uint32_t flags)> named_buffer_storage{ "glNamedBufferStorage" };
	inline c_opengl_export<void(std::uint32_t buffer, std::intptr_t size, const void* data, std::uint32_t usage)> named_buffer_data{ "glNamedBufferData" };
	inline c_opengl_export<void(std::uint32_t buffer, std::intptr_t offset, std::intptr_t size, const void* data)> named_buffer_sub_data{ "glNamedBufferSubData" };
	inline c_opengl_export<void(std::uint32_t read_buffer, std::uint32_t write_buffer, std::intptr_t read_offset, std::intptr_t write_offset, std::intptr_t size)> copy_named_buffer_sub_data{ "glCopyNamedBufferSubData" };
	inline c_opengl_export<void(std::uint32_t buffer, std::uint32_t internalformat, std::uint32_t format, std::uint32_t type, const void* data)> clear_named_buffer_data{ "glClearNamedBufferData" };
	inline c_opengl_export<void(std::uint32_t buffer, std::uint32_t internalformat, std::intptr_t offset, std::intptr_t size, std::uint32_t format, std::uint32_t type, const void* data)> clear_named_buffer_sub_data{ "glClearNamedBufferSubData" };
	inline c_opengl_export<void*(std::uint32_t buffer, std::uint32_t access)> map_named_buffer{ "glMapNamedBuffer" };
	inline c_opengl_export<void*(std::uint32_t buffer, std::intptr_t offset, std::intptr_t length, std::uint32_t access)> map_named_buffer_range{ "glMapNamedBufferRange" };
	inline c_opengl_export<bool(std::uint32_t buffer)> unmap_named_buffer{ "glUnmapNamedBuffer" };
	inline c_opengl_export<void(std::uint32_t buffer, std::intptr_t offset, std::intptr_t length)> flush_mapped_named_buffer_range{ "glFlushMappedNamedBufferRange" };
	inline c_opengl_export<void(std::uint32_t buffer, std::uint32_t pname, int* params)> get_named_buffer_parameteriv{ "glGetNamedBufferParameteriv" };
	inline c_opengl_export<void(std::uint32_t buffer, std::uint32_t pname, std::int64_t* params)> get_named_buffer_parameteri64v{ "glGetNamedBufferParameteri64v" };
	inline c_opengl_export<void(std::uint32_t buffer, std::uint32_t pname, void** params)> get_named_buffer_pointerv{ "glGetNamedBufferPointerv" };
	inline c_opengl_export<void(std::uint32_t buffer, std::intptr_t offset, std::intptr_t size, void* data)> get_named_buffer_sub_data{ "glGetNamedBufferSubData" };
	inline c_opengl_export<void(int n, std::uint32_t* framebuffers)> create_framebuffers{ "glCreateFramebuffers" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t attachment, std::uint32_t renderbuffertarget, std::uint32_t renderbuffer)> named_framebuffer_renderbuffer{ "glNamedFramebufferRenderbuffer" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t pname, int param)> named_framebuffer_parameteri{ "glNamedFramebufferParameteri" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t attachment, std::uint32_t texture, int level)> named_framebuffer_texture{ "glNamedFramebufferTexture" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t attachment, std::uint32_t texture, int level, int layer)> named_framebuffer_texture_layer{ "glNamedFramebufferTextureLayer" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t buf)> named_framebuffer_draw_buffer{ "glNamedFramebufferDrawBuffer" };
	inline c_opengl_export<void(std::uint32_t framebuffer, int n, const std::uint32_t* bufs)> named_framebuffer_draw_buffers{ "glNamedFramebufferDrawBuffers" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t src)> named_framebuffer_read_buffer{ "glNamedFramebufferReadBuffer" };
	inline c_opengl_export<void(std::uint32_t framebuffer, int num_attachments, const std::uint32_t* attachments)> invalidate_named_framebuffer_data{ "glInvalidateNamedFramebufferData" };
	inline c_opengl_export<void(std::uint32_t framebuffer, int num_attachments, const std::uint32_t* attachments, int x, int y, int width, int height)> invalidate_named_framebuffer_sub_data{ "glInvalidateNamedFramebufferSubData" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t buffer, int drawbuffer, const int* value)> clear_named_framebufferiv{ "glClearNamedFramebufferiv" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t buffer, int drawbuffer, const std::uint32_t* value)> clear_named_framebufferuiv{ "glClearNamedFramebufferuiv" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t buffer, int drawbuffer, const float* value)> clear_named_framebufferfv{ "glClearNamedFramebufferfv" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t buffer, int drawbuffer, float depth, int stencil)> clear_named_framebufferfi{ "glClearNamedFramebufferfi" };
	inline c_opengl_export<void(std::uint32_t read_framebuffer, std::uint32_t draw_framebuffer, int src_x0, int src_y0, int src_x1, int src_y1, int dst_x0, int dst_y0, int dst_x1, int dst_y1, std::uint32_t mask, std::uint32_t filter)> blit_named_framebuffer{ "glBlitNamedFramebuffer" };
	inline c_opengl_export<std::uint32_t(std::uint32_t framebuffer, std::uint32_t target)> check_named_framebuffer_status{ "glCheckNamedFramebufferStatus" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t pname, int* param)> get_named_framebuffer_parameteriv{ "glGetNamedFramebufferParameteriv" };
	inline c_opengl_export<void(std::uint32_t framebuffer, std::uint32_t attachment, std::uint32_t pname, int* params)> get_named_framebuffer_attachment_parameteriv{ "glGetNamedFramebufferAttachmentParameteriv" };
	inline c_opengl_export<void(int n, std::uint32_t* renderbuffers)> create_renderbuffers{ "glCreateRenderbuffers" };
	inline c_opengl_export<void(std::uint32_t renderbuffer, std::uint32_t internalformat, int width, int height)> named_renderbuffer_storage{ "glNamedRenderbufferStorage" };
	inline c_opengl_export<void(std::uint32_t renderbuffer, int samples, std::uint32_t internalformat, int width, int height)> named_renderbuffer_storage_multisample{ "glNamedRenderbufferStorageMultisample" };
	inline c_opengl_export<void(std::uint32_t renderbuffer, std::uint32_t pname, int* params)> get_named_renderbuffer_parameteriv{ "glGetNamedRenderbufferParameteriv" };
	inline c_opengl_export<void(std::uint32_t target, int n, std::uint32_t* textures)> create_textures{ "glCreateTextures" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t internalformat, std::uint32_t buffer)> texture_buffer{ "glTextureBuffer" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t internalformat, std::uint32_t buffer, std::intptr_t offset, std::intptr_t size)> texture_buffer_range{ "glTextureBufferRange" };
	inline c_opengl_export<void(std::uint32_t texture, int levels, std::uint32_t internalformat, int width)> texture_storage1d{ "glTextureStorage1D" };
	inline c_opengl_export<void(std::uint32_t texture, int levels, std::uint32_t internalformat, int width, int height)> texture_storage2d{ "glTextureStorage2D" };
	inline c_opengl_export<void(std::uint32_t texture, int levels, std::uint32_t internalformat, int width, int height, int depth)> texture_storage3d{ "glTextureStorage3D" };
	inline c_opengl_export<void(std::uint32_t texture, int samples, std::uint32_t internalformat, int width, int height, bool fixedsamplelocations)> texture_storage2d_multisample{ "glTextureStorage2DMultisample" };
	inline c_opengl_export<void(std::uint32_t texture, int samples, std::uint32_t internalformat, int width, int height, int depth, bool fixedsamplelocations)> texture_storage3d_multisample{ "glTextureStorage3DMultisample" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int width, std::uint32_t format, std::uint32_t type, const void* pixels)> texture_sub_image1d{ "glTextureSubImage1D" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int width, int height, std::uint32_t format, std::uint32_t type, const void* pixels)> texture_sub_image2d{ "glTextureSubImage2D" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, std::uint32_t format, std::uint32_t type, const void* pixels)> texture_sub_image3d{ "glTextureSubImage3D" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int width, std::uint32_t format, int image_size, const void* data)> compressed_texture_sub_image1d{ "glCompressedTextureSubImage1D" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int width, int height, std::uint32_t format, int image_size, const void* data)> compressed_texture_sub_image2d{ "glCompressedTextureSubImage2D" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, std::uint32_t format, int image_size, const void* data)> compressed_texture_sub_image3d{ "glCompressedTextureSubImage3D" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int x, int y, int width)> copy_texture_sub_image1d{ "glCopyTextureSubImage1D" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int x, int y, int width, int height)> copy_texture_sub_image2d{ "glCopyTextureSubImage2D" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height)> copy_texture_sub_image3d{ "glCopyTextureSubImage3D" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, float param)> texture_parameterf{ "glTextureParameterf" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, const float* param)> texture_parameterfv{ "glTextureParameterfv" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, int param)> texture_parameteri{ "glTextureParameteri" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, const int* params)> texture_parameter_iiv{ "glTextureParameterIiv" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, const std::uint32_t* params)> texture_parameter_iuiv{ "glTextureParameterIuiv" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, const int* param)> texture_parameteriv{ "glTextureParameteriv" };
	inline c_opengl_export<void(std::uint32_t texture)> generate_texture_mipmap{ "glGenerateTextureMipmap" };
	inline c_opengl_export<void(std::uint32_t unit, std::uint32_t texture)> bind_texture_unit{ "glBindTextureUnit" };
	inline c_opengl_export<void(std::uint32_t texture, int level, std::uint32_t format, std::uint32_t type, int buf_size, void* pixels)> get_texture_image{ "glGetTextureImage" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int buf_size, void* pixels)> get_compressed_texture_image{ "glGetCompressedTextureImage" };
	inline c_opengl_export<void(std::uint32_t texture, int level, std::uint32_t pname, float* params)> get_texture_level_parameterfv{ "glGetTextureLevelParameterfv" };
	inline c_opengl_export<void(std::uint32_t texture, int level, std::uint32_t pname, int* params)> get_texture_level_parameteriv{ "glGetTextureLevelParameteriv" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, float* params)> get_texture_parameterfv{ "glGetTextureParameterfv" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, int* params)> get_texture_parameter_iiv{ "glGetTextureParameterIiv" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, std::uint32_t* params)> get_texture_parameter_iuiv{ "glGetTextureParameterIuiv" };
	inline c_opengl_export<void(std::uint32_t texture, std::uint32_t pname, int* params)> get_texture_parameteriv{ "glGetTextureParameteriv" };
	inline c_opengl_export<void(int n, std::uint32_t* arrays)> create_vertex_arrays{ "glCreateVertexArrays" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t index)> disable_vertex_array_attrib{ "glDisableVertexArrayAttrib" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t index)> enable_vertex_array_attrib{ "glEnableVertexArrayAttrib" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t buffer)> vertex_array_element_buffer{ "glVertexArrayElementBuffer" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t bindingindex, std::uint32_t buffer, std::intptr_t offset, int stride)> vertex_array_vertex_buffer{ "glVertexArrayVertexBuffer" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t first, int count, const std::uint32_t* buffers, const std::intptr_t* offsets, const int* strides)> vertex_array_vertex_buffers{ "glVertexArrayVertexBuffers" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t attribindex, std::uint32_t bindingindex)> vertex_array_attrib_binding{ "glVertexArrayAttribBinding" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t attribindex, int size, std::uint32_t type, bool normalized, std::uint32_t relativeoffset)> vertex_array_attrib_format{ "glVertexArrayAttribFormat" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t attribindex, int size, std::uint32_t type, std::uint32_t relativeoffset)> vertex_array_attrib_i_format{ "glVertexArrayAttribIFormat" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t attribindex, int size, std::uint32_t type, std::uint32_t relativeoffset)> vertex_array_attrib_l_format{ "glVertexArrayAttribLFormat" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t bindingindex, std::uint32_t divisor)> vertex_array_binding_divisor{ "glVertexArrayBindingDivisor" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t pname, int* param)> get_vertex_arrayiv{ "glGetVertexArrayiv" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t index, std::uint32_t pname, int* param)> get_vertex_array_indexediv{ "glGetVertexArrayIndexediv" };
	inline c_opengl_export<void(std::uint32_t vaobj, std::uint32_t index, std::uint32_t pname, std::int64_t* param)> get_vertex_array_indexed64iv{ "glGetVertexArrayIndexed64iv" };
	inline c_opengl_export<void(int n, std::uint32_t* samplers)> create_samplers{ "glCreateSamplers" };
	inline c_opengl_export<void(int n, std::uint32_t* pipelines)> create_program_pipelines{ "glCreateProgramPipelines" };
	inline c_opengl_export<void(std::uint32_t target, int n, std::uint32_t* ids)> create_queries{ "glCreateQueries" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t buffer, std::uint32_t pname, std::intptr_t offset)> get_query_buffer_objecti64v{ "glGetQueryBufferObjecti64v" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t buffer, std::uint32_t pname, std::intptr_t offset)> get_query_buffer_objectiv{ "glGetQueryBufferObjectiv" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t buffer, std::uint32_t pname, std::intptr_t offset)> get_query_buffer_objectui64v{ "glGetQueryBufferObjectui64v" };
	inline c_opengl_export<void(std::uint32_t id, std::uint32_t buffer, std::uint32_t pname, std::intptr_t offset)> get_query_buffer_objectuiv{ "glGetQueryBufferObjectuiv" };
	inline c_opengl_export<void(std::uint32_t barriers)> memory_barrier_by_region{ "glMemoryBarrierByRegion" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, std::uint32_t format, std::uint32_t type, int buf_size, void* pixels)> get_texture_sub_image{ "glGetTextureSubImage" };
	inline c_opengl_export<void(std::uint32_t texture, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, int buf_size, void* pixels)> get_compressed_texture_sub_image{ "glGetCompressedTextureSubImage" };
	inline c_opengl_export<std::uint32_t()> get_graphics_reset_status{ "glGetGraphicsResetStatus" };
	inline c_opengl_export<void(std::uint32_t target, int lod, int buf_size, void* pixels)> getn_compressed_tex_image{ "glGetnCompressedTexImage" };
	inline c_opengl_export<void(std::uint32_t target, int level, std::uint32_t format, std::uint32_t type, int buf_size, void* pixels)> getn_tex_image{ "glGetnTexImage" };
	inline c_opengl_export<void(std::uint32_t program, int location, int buf_size, double* params)> getn_uniformdv{ "glGetnUniformdv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int buf_size, float* params)> getn_uniformfv{ "glGetnUniformfv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int buf_size, int* params)> getn_uniformiv{ "glGetnUniformiv" };
	inline c_opengl_export<void(std::uint32_t program, int location, int buf_size, std::uint32_t* params)> getn_uniformuiv{ "glGetnUniformuiv" };
	inline c_opengl_export<void(int x, int y, int width, int height, std::uint32_t format, std::uint32_t type, int buf_size, void* data)> readn_pixels{ "glReadnPixels" };
	inline c_opengl_export<void()> texture_barrier{ "glTextureBarrier" };
	inline c_opengl_export<void(std::uint32_t shader, const char* p_entry_point, std::uint32_t num_specialization_constants, const std::uint32_t* p_constant_index, const std::uint32_t* p_constant_value)> specialize_shader{ "glSpecializeShader" };
	inline c_opengl_export<void(std::uint32_t mode, const void* indirect, std::intptr_t drawcount, int maxdrawcount, int stride)> multi_draw_arrays_indirect_count{ "glMultiDrawArraysIndirectCount" };
	inline c_opengl_export<void(std::uint32_t mode, std::uint32_t type, const void* indirect, std::intptr_t drawcount, int maxdrawcount, int stride)> multi_draw_elements_indirect_count{ "glMultiDrawElementsIndirectCount" };
	inline c_opengl_export<void(float factor, float units, float clamp)> polygon_offset_clamp{ "glPolygonOffsetClamp" };
}