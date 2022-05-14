export module null.render:draw_list;
import :font;
import null.sdk;

export namespace null {
	enum struct e_draw_list_flags : int {
		anti_aliased_lines = 1 << 0,
		anti_aliased_lines_use_texture = 1 << 1,
		anti_aliased_fill = 1 << 2,
		allow_vtx_offset = 1 << 3,
		alla = anti_aliased_lines_use_texture | anti_aliased_fill
	}; enum_create_bit_operators(e_draw_list_flags, true);

	enum class e_corner_flags {
		top_left = 1 << 0,
		top_right = 1 << 1,
		bot_left = 1 << 2,
		bot_right = 1 << 3,

		top = top_left | top_right,
		bot = bot_left | bot_right,
		left = top_left | bot_left,
		right = top_right | top_right,

		all = top_left | top_right | bot_left | bot_right
	}; enum_create_bit_operators(e_corner_flags, true);

	enum class e_cmd_callbacks {
		render_draw_data //std::function<bool(cmd*)>, will call setup_render_state if the callback returns true
	};

	namespace render {
		struct shared_data_t {
			e_draw_list_flags initialize_flags{ };
			c_font* font{ };
			rect_t clip_rect_fullscreen{ };

			std::array<vec2_t, 12> arc_fast_vtx{ };
			std::array<std::uint8_t, 64> circle_segments{ };
			float circle_segment_max_error{ };
			float curve_tessellation_tol{ };

			shared_data_t(e_draw_list_flags _initialize_flags = e_draw_list_flags{ }) : initialize_flags(_initialize_flags) {
				for(int i = 0; i < arc_fast_vtx.size(); i++) {
					float a = (i * 2.f * std::numbers::pi) / arc_fast_vtx.size();
					arc_fast_vtx[i] = vec2_t{ std::cosf(a), std::sinf(a) };
				}
			}

			void set_circle_segment_max_error(float max_error) {
				if(circle_segment_max_error == max_error) return;
				circle_segment_max_error = max_error;

				for(int i = 0; i < circle_segments.size(); i++) {
					float radius = i + 1.f;
					int segment_count = std::clamp((std::numbers::pi * 2.f) / std::acosf((radius - circle_segment_max_error) / radius), 12., 512.);
					circle_segments[i] = (std::uint8_t)std::min(segment_count, 255);
				}
			}
		} shared_data{ e_draw_list_flags::allow_vtx_offset };

		class c_draw_list {
		public:
			struct vertex_t {
				vec2_t pos{ }, uv{ };
				color_t color{ };
			};

			struct draw_data_t {
				std::vector<c_draw_list*> layers{ };

				bool valid{ };
				std::vector<c_draw_list*> cmd_lists{ };
				int total_idx_count{ }, total_vtx_count{ };

				vec2_t window_pos{ }, window_size{ };

				void de_index_all_buffers();
				void add_draw_list(c_draw_list* draw_list);
				void setup();

				void scale_clip_rects(vec2_t scale) {
					for(c_draw_list* cmd_list : cmd_lists)
						for(cmd_t& cmd : cmd_list->cmd_buffer) cmd.clip_rect *= scale;
				}
			};

			struct cmd_header_t {
				rect_t clip_rect{ };
				void* texture_id{ };
				std::uint32_t vtx_offset{ };

				class_create_spaceship_operator(cmd_header_t);
			};

			struct cmd_t : cmd_header_t {
				std::uint32_t idx_offset{ }, element_count{ };

				single_callbacks_t<e_cmd_callbacks> callbacks{ };
			};

		public:
			shared_data_t* shared_data{ };

			e_draw_list_flags flags{ };
			std::vector<void*> texture_id_stack{ };
			std::vector<vec2_t> path{ };
			std::vector<rect_t> clip_rect_stack{ };

			std::vector<cmd_t> cmd_buffer{ };
			cmd_header_t cmd_header{ };

			std::vector<std::uint16_t> idx_buffer{ };
			std::vector<std::uint16_t>::iterator idx_write{ };

			std::vector<vertex_t> vtx_buffer{ };
			std::vector<vertex_t>::iterator vtx_write{ };
			std::uint32_t vtx_current_idx{ };

		public:
			void on_changed_clip_rect();
			void push_clip_rect(rect_t rect, bool intersect_with_current_rect = false);
			void push_clip_rect_fullscreen() { push_clip_rect(shared_data->clip_rect_fullscreen); }
			void pop_clip_rect();

			void on_changed_texture_id();
			void push_texture_id(void* texture_id);
			void pop_texture_id();

			void shade_verts_linear_uv(std::vector<vertex_t>::iterator vtx_start, std::vector<vertex_t>::iterator vtx_end, rect_t rect, rect_t uv, bool clamp);

			void on_changed_vtx_offset();
			void prim_reserve(int idx_count, int vtx_count);
			void prim_unreserve(int idx_count, int vtx_count);
			void prim_rect(vec2_t a, vec2_t c, color_t color);
			void prim_rect_uv(vec2_t a, vec2_t c, vec2_t uv_a, vec2_t uv_c, color_t color);
			void prim_quad_uv(std::array<vec2_t, 4> points, std::array<vec2_t, 4> uvs, color_t color);
			
			void prim_write_vtx(std::vector<vertex_t> vtx_list) { std::move(vtx_list.begin(), vtx_list.end(), vtx_write); vtx_write += vtx_list.size(); vtx_current_idx += vtx_list.size(); }
			void prim_write_vtx(vertex_t vtx) { *vtx_write = vtx; vtx_write++; vtx_current_idx++; }	
			void prim_write_idx(std::vector<std::uint16_t> idx_list) { std::move(idx_list.begin(), idx_list.end(), idx_write); idx_write += idx_list.size(); }
			void prim_write_idx(std::uint16_t idx) { *idx_write = idx; idx_write++; }
			void prim_vtx(vertex_t vtx) { prim_write_idx((std::uint16_t)vtx_current_idx); prim_write_vtx(vtx); }

			void pop_unused_draw_cmd() {
				if(cmd_buffer.empty()) return;
				if(!cmd_buffer.back().element_count && !cmd_buffer.back().callbacks.empty()) cmd_buffer.pop_back();
			}
			
			void add_draw_cmd() { cmd_buffer.push_back({ cmd_header.clip_rect, cmd_header.texture_id, cmd_header.vtx_offset, (std::uint32_t)idx_buffer.size() }); }
			void reset_for_begin_render() {
				cmd_buffer.clear(); cmd_buffer.push_back(cmd_t{ });
				idx_buffer.clear();
				vtx_buffer.clear();
				clip_rect_stack.clear();
				texture_id_stack.clear();
				path.clear();

				flags = shared_data->initialize_flags;
				cmd_header = cmd_header_t{ };
				vtx_current_idx = 0;
			}
		};

		c_draw_list::draw_data_t draw_data{ };
		c_draw_list background_draw_list{ }, foreground_draw_list{ };
	}
}