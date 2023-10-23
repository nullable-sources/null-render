#pragma once
#include <null-sdk.h>

#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#include <stb_rect_pack.h>

#include "loaders/loader.h"

constexpr auto c = sizeof(std::string_view::const_iterator);
namespace null::render {
	namespace impl {
		static int get_char_from_utf8(std::uint32_t* out_char, std::string_view::const_iterator iterator, std::string_view::const_iterator end) {
			static constexpr std::array<char, 32> lengths{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0 };
			static constexpr std::array<int, 5> masks{ 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
			static constexpr std::array<std::uint32_t, 5> mins{ 0x400000, 0, 0x80, 0x800, 0x10000 };
			static constexpr std::array<int, 5> shiftc{ 0, 18, 12, 6, 0 };
			static constexpr std::array<int, 5> shifte{ 0, 6, 4, 2, 0 };
			int len = lengths[*(const unsigned char*)iterator._Unwrapped() >> 3];
			int wanted = len + !len;

			std::array<std::uint8_t, 4> s{ };
			for(int i = 0; std::next(iterator, i) != end && i < s.size(); i++) {
				s[i] = *std::next(iterator, i);
			}

			*out_char = (std::uint32_t)(s[0] & masks[len]) << 18;
			*out_char |= (std::uint32_t)(s[1] & 0x3f) << 12;
			*out_char |= (std::uint32_t)(s[2] & 0x3f) << 6;
			*out_char |= (std::uint32_t)(s[3] & 0x3f) << 0;
			*out_char >>= shiftc[len];

			int e = (*out_char < mins[len]) << 6;
			e |= ((*out_char >> 11) == 0x1b) << 7;
			e |= (*out_char > 0xFFFF) << 8;
			e |= (s[1] & 0xc0) >> 2;
			e |= (s[2] & 0xc0) >> 4;
			e |= (s[3]) >> 6;
			e ^= 0x2a;
			e >>= shifte[len];

			if(e) {
				wanted = std::min(wanted, !!s[0] + !!s[1] + !!s[2] + !!s[3]);
				*out_char = 0xFFFD;
			}

			return wanted;
		}

		namespace char_converters {
			template <typename string_t> struct char_t { using type = string_t::value_type; };
			template <> struct char_t<const char*> { using type = char; };
			template <> struct char_t<const wchar_t*> { using type = char; };

			template <class string_t>
			struct converter {
				template <typename iterator_t>
				static int convert(std::uint32_t& output_char, const iterator_t& iterator, const iterator_t& end) {
					return 1;
				}
			};

			template <>
			struct converter<char> {
				static int convert(std::uint32_t& output_char, const std::string_view::const_iterator& iterator, const std::string_view::const_iterator& end) {
					return output_char < 0x80 ? 1 : get_char_from_utf8(&output_char, iterator, end);
				}
			};
		}
	}

	enum class e_rasterizer_flags {
		no_hinting = 1 << 0,
		no_auto_hint = 1 << 1,
		force_auto_hint = 1 << 2,
		light_hinting = 1 << 3,
		mono_hinting = 1 << 4,
		bold = 1 << 5,
		oblique = 1 << 6
	}; enum_create_bit_operators(e_rasterizer_flags, true);
	enum_create_cast_operator(e_rasterizer_flags, -);

	enum class e_render_mode_type {
		monochrome,
		sdf
	};

	class c_atlas;
	class c_font {
	public:
		struct glyph_t {
		public:
			struct config_t {
				vec2_t<float> offset{ }, extra_spacing{ };
				const std::uint16_t* ranges{ };
				float min_advance_x, max_advance_x{ std::numeric_limits<float>::max() };
			};

		public:
			std::uint32_t codepoint{ 31 };
			bool visible{ true };
			float advance_x{ };
			rect_t<float> corners{ }, texture_coordinates{ };

		public:
			static const std::uint16_t* ranges_default() {
				static const std::uint16_t ranges[] = {
					0x0020, 0x00FF,
					0,
				}; return &ranges[0];
			}

			static const std::uint16_t* ranges_cyrillic() {
				static const std::uint16_t ranges[] = {
					0x0020, 0x00FF,
					0x0400, 0x052F,
					0x2DE0, 0x2DFF,
					0xA640, 0xA69F,
					0,
				};
				return &ranges[0];
			}
		};

	public:
		struct config_t {
		public:
			c_font* font{ };
			glyph_t::config_t glyph_config{ };

			std::vector<char> data{ };
			int index{ };
			float size_pixels{ };
			vec2_t<float> oversample{ 3, 1 };
			bool pixel_snap_h{ };

			e_rasterizer_flags rasterizer_flags{ };
			e_render_mode_type render_mode_type{ };
		};

		struct lookup_table_t {
		public:
			std::vector<float> advances_x{ };
			std::vector<std::uint16_t> indexes{ };
			bool dirty{ };

		public:
			void resize(int new_size) {
				if(advances_x.size() != indexes.size()) utils::logger(utils::e_log_type::warning, "advances_x.size() != indexes.size().");
				if(new_size <= indexes.size()) return;
				advances_x.resize(new_size, -1.0f);
				indexes.resize(new_size, (std::uint16_t)-1);
			}
		};

	public:
		static inline c_font* default_font{ }; //@note: set only from set_current_font

	public:
		std::uint16_t fallback_char{ (std::uint16_t)'?' };

		lookup_table_t lookup_table{ };
		std::vector<glyph_t> glyphs{ };
		glyph_t* fallback_glyph{ };
		float fallback_advance_x{ };

		c_atlas* container_atlas{ };
		config_t* config{ };

		float size{ };
		float ascent{ }, descent{ };

	public:
		void build_lookup_table();

		glyph_t* find_glyph(std::uint16_t c, bool fallback = true);
		void add_glyph(config_t* src_config, std::uint16_t c, rect_t<float> corners, const rect_t<float>& texture_coordinates, float advance_x);

		void set_fallback_char(std::uint16_t c) { fallback_char = c; build_lookup_table(); }
		void set_glyph_visible(std::uint16_t c, bool visible) { if(glyph_t* glyph = find_glyph(c)) glyph->visible = visible; }

		bool is_loaded() const { return container_atlas; }
		float get_char_advance(std::uint16_t c) const { return (c < lookup_table.advances_x.size()) ? lookup_table.advances_x[c] : fallback_advance_x; }

		template <typename string_t> vec2_t<float> calc_text_size(const string_t& text, float custom_size = 0.f) { return calc_text_size(std::basic_string_view(text), custom_size); }
		template <typename char_t> vec2_t<float> calc_text_size(std::basic_string_view<char_t> text, float custom_size = 0.f);
	};

	class c_atlas {
	public:
		struct custom_rect_t {
		public:
			rect_t<float> size{ vec2_t{ std::numeric_limits<std::uint16_t>::max() }, vec2_t{ 0.f } };
			std::uint32_t glyph_id{ };
			float glyph_advance_x{ };
			vec2_t<float> glyph_offset{ };
			c_font* font{ };

		public:
			bool is_packed() const { return size.min.x != std::numeric_limits<std::uint16_t>::max(); }
		};

	public:
		struct texture_t {
		public:
			void* data{ };
			int desired_width{ 0 }, glyph_padding{ 1 };

			std::vector<std::uint8_t> pixels_alpha8{ };
			std::vector<std::uint32_t> pixels_rgba32{ };

			vec2_t<float> size{ };

		public:
			void clear() { pixels_alpha8.clear(); pixels_rgba32.clear(); }
			bool is_built() const { return !pixels_alpha8.empty() || !pixels_rgba32.empty(); }
			void get_data_as_rgba32();
		};

	public:
		texture_t texture{ };

		bool locked{ };
		std::vector<std::unique_ptr<c_font>> fonts{ };
		std::vector<c_font::config_t> configs{ };

		std::unique_ptr<i_font_loader> font_loader{ };

	public:
		~c_atlas() { clear(); }

	public:
		void setup_font(c_font* font, c_font::config_t* config, float ascent, float descent);

		void build_finish();

		void build();

		c_font* add_font(c_font::config_t* config);
		c_font* add_font_default(c_font::config_t* config = nullptr);
		c_font* add_font_from_file_ttf(std::string_view filename, float size_pixels, c_font::config_t* config = nullptr, const std::uint16_t* glyph_ranges = c_font::glyph_t::ranges_default());
		c_font* add_font_from_memory_ttf(const std::vector<char>& font_data, float size_pixels, c_font::config_t* config = nullptr, const std::uint16_t* glyph_ranges = c_font::glyph_t::ranges_default());
		c_font* add_font_from_memory_compressed_ttf(const std::vector<std::uint8_t>& compressed_ttf, float size_pixels, c_font::config_t* config = nullptr, const std::uint16_t* glyph_ranges = c_font::glyph_t::ranges_default());

		void clear_input_data();
		void clear() { clear_input_data(); texture.clear(); fonts.clear(); }
	} inline atlas{ };

	struct atlases_handler_t {
	public:
		bool changed{ true };
		std::vector<c_atlas*> atlases{ &atlas };

	public:
		void add(c_atlas* atlas) {
			atlases.push_back(atlas);
			changed = true;
		}
	} inline atlases_handler{ };

	c_font* get_default_font();
	void set_default_font(c_font* font);
}

#include "font.inl"