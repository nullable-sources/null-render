#pragma once
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC

#define sdf_pixel_size 40
#define sdf_padding (sdf_pixel_size / 10)

#include "../loader.h"
#include <stb_truetype.h>

namespace null::render {
	class c_truetype_loader : public i_font_loader {
	public:
		struct build_data_t {
			int glyphs_highest{ }, glyphs_count{ };
			std::vector<std::uint32_t> glyphs_set{ };
		};

		struct build_src_t : build_data_t {
			stbtt_fontinfo font_info{ };
			stbtt_pack_range pack_range{ };
			stbrp_rect* rects{ };
			stbtt_packedchar* packed_chars{ };

			const std::uint16_t* src_ranges{ };
			int dst_index{ };
			std::vector<int> glyphs_list{ };
		};

	public:
		void build(c_atlas* atlas) override;
	};
}