#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftmodapi.h>
#include <freetype/ftglyph.h>
#include <freetype/ftsynth.h>

#include "font/font.h"
#include "font/loaders/loader.h"

namespace null::render {
	class c_freetype_loader : public i_font_loader {
	public:
		struct build_data_t {
		public:
			int glyphs_highest{ }, glyphs_count{ };
			std::vector<std::uint32_t> glyphs_set{ };
		};

		struct src_glyph_t {
		public:
			c_font::glyph_t glyph{ };
			std::vector<std::uint8_t> bitmap{ };
		};

		struct build_src_t : build_data_t {
		public:
			struct freetype_t {
			public:
				struct info_t {
				public:
					std::uint32_t pixel_height{ };
					float ascender{ }, descender{ }, line_spacing{ }, line_gap{ }, max_advance_width{ };
				};

			public:
				info_t info{ };
				FT_Face face{ };
				e_rasterizer_flags rasterizer_flags{ };
				FT_Int32 flags{ };
				FT_Render_Mode render_mode{ };
			};

		public:
			freetype_t freetype{ };

			stbrp_rect* rects{ };

			const std::uint16_t* src_ranges{ };
			int dst_index{ };
			std::vector<src_glyph_t> glyphs_list{ };
		};

	public:
		void build(c_atlas* atlas) override;
	};
}