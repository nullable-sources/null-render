#define MSDF_ATLAS_STANDALONE
#define MSDF_ATLAS_PUBLIC
#define MSDFGEN_USE_CPP11
#define MSDFGEN_PUBLIC
#define MSDFGEN_EXTENSIONS
#define MSDFGEN_DISABLE_SVG
#define MSDFGEN_EXT_PUBLIC

#include <functional>
#include <thread>
#include <print>
#include <freetype/freetype.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <artery-font-format/artery-font/artery-font.h>
#include <artery-font-format/artery-font/std-artery-font.h>

#include "../../font.h"
#include "freetype.h"

namespace null::render {
    struct c_freetype_loader::font_data_t {
    private:
        template <typename T>
        struct std_list_t : public std::vector<T> {

        public:
            inline std_list_t() { }
            inline explicit std_list_t(int length) : std::vector<T>((size_t)length) { }
            inline int length() const { return (int)std::vector<T>::size(); }
            inline explicit operator T* () { return std::vector<T>::data(); }
            inline explicit operator const T* () const { return std::vector<T>::data(); }
            inline T& operator[](int index) { return std::vector<T>::operator[](index); }
            inline const T& operator[](int index) const { return std::vector<T>::operator[](index); }

        };

        typedef std_list_t<std::uint8_t> std_byte_array;
        using ar_font_t = artery_font::ArteryFont<float, std_list_t, std_byte_array, artery_font::StdString>;

    public:
        struct artery_data_t {
        private:
            struct reader_t {
            public:
                std::vector<std::uint8_t>* file{ };
                size_t read_position{ };
            };

            static int artery_read(void* buffer, int lenght, void* data) {
                reader_t* reader = (reader_t*)data;
                size_t bytes = std::min((size_t)lenght, reader->file->size() - reader->read_position);
                if(!bytes) return 0;

                if(buffer) {
                    for(size_t bytes_left = bytes, copy_size{ }; bytes_left > 0; copy_size = std::min(bytes_left, reader->file->size() - reader->read_position)) {
                        buffer = (void*)((std::uintptr_t)std::memcpy(buffer, reader->file->data() + reader->read_position, copy_size) + copy_size);

                        reader->read_position += copy_size; bytes_left -= copy_size;
                    }
                } else reader->read_position += bytes;

                return bytes;
            }

        private:
            ar_font_t::Image image{ };
            ar_font_t::Variant variant{ };

        public:
            artery_data_t() { }

        public:
            bool load(std::vector<std::uint8_t>& data) {
                ar_font_t ar_font{ };
                reader_t reader(&data);
                artery_font::decode<artery_read>(ar_font, &reader);

                if(ar_font.images.empty() || ar_font.variants.empty()) {
                    utils::logger(utils::e_log_type::error, "artery_font images or variants empty, font loading canceled.");
                    return false;
                }

                image = ar_font.images[0];
                variant = ar_font.variants[0];

                if(image.pixelFormat != artery_font::PIXEL_FLOAT32) {
                    utils::logger(utils::e_log_type::error, "unsupported artery font pixel format.");
                    return false;
                }

                if(image.rawBinaryFormat.orientation != artery_font::ORIENTATION_BOTTOM_UP) {
                    utils::logger(utils::e_log_type::error, "unsupported artery font orientation.");
                    return false;
                }
                
                return true;
            }

        public:
            artery_font::Glyph<float>* find_glyph(std::uint32_t codepoint) {
                if(auto finded = std::ranges::find(variant.glyphs, codepoint, &artery_font::Glyph<float>::codepoint); finded != variant.glyphs.end())
                    return &*finded;
                return nullptr;
            }

            size_t image_channels() { return image.channels; }
            float* image_read(size_t x, size_t y) {
                return (float*)(image.data.data() + (image_channels() * sizeof(float)) * (image.width * y + x));
            }
        };

    public:
        font_config_t* config{ };
        artery_data_t artery_data{ };
        std::vector<msdf_atlas::GlyphGeometry> glyphs{ };

    public:
        font_data_t(font_config_t* _config) : config(_config) {  }
    };

    void c_freetype_loader::build(c_atlas* atlas) {
        if(atlas->locked) { utils::logger(utils::e_log_type::error, "cannot modify a locked atlas between begin_render() and end_render/render()."); return; }
        if(atlas->configs.empty()) { utils::logger(utils::e_log_type::warning, "configs array is empty."); return; }

        std::vector<font_data_t> fonts{ };
        for(font_config_t& config : atlas->configs)
            fonts.push_back(font_data_t(&config));

        //@note: @note: we prepare fonts for packaging
        initialize_fonts(fonts); //@note: we load the necessary glyphs and information about them, artety_font (if one is loaded in the config)

        //@note: packing the glyphs and resizing the atlas
        atlas->texture.resize(pack_fonts(fonts));

        //@note: Rasterize the glyphs.
        //       If artery_font is loaded, then we use the already generated glyphs and just copy them to our atlas,
        //       if artery_font is not loaded, then we generate the glyphs ourselves
        rasterize_fonts(fonts, atlas->texture);

        //@note: the final pass through the loadable fonts.
        //       Setting up glyph data for rendering and font metrics.
        build_fonts(fonts);
    }

    void c_freetype_loader::initialize_fonts(std::vector<font_data_t>& fonts) {
        msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
        if(!ft) {
            utils::logger(utils::e_log_type::error, "msdfgen failed to initialize freetype.");
            return;
        }

        for(font_data_t& font : fonts) {
            msdfgen::FontHandle* ft_font = msdfgen::loadFontData(ft, font.config->font_data.data(), font.config->font_data.size());
            if(!ft_font) {
                utils::logger(utils::e_log_type::error, "msdfgen failed to load font data.");
                continue;
            }

            if(!font.config->artery_data.empty() && !font.artery_data.load(font.config->artery_data))
                continue;

            msdf_atlas::FontGeometry font_geometry = msdf_atlas::FontGeometry(&font.glyphs);

            msdf_atlas::Charset loading_charset{ };
            for(auto& [range_start, range_end] : font.config->charsets)
                std::ranges::for_each(std::views::iota(range_start, range_end), std::bind(&msdf_atlas::Charset::add, &loading_charset, std::placeholders::_1));

            std::uint32_t font_flags{ };
            if(font.config->rasterizer_flags & e_font_rasterizer_flags::no_hinting) font_flags |= FT_LOAD_NO_HINTING;
            if(font.config->rasterizer_flags & e_font_rasterizer_flags::no_auto_hint) font_flags |= FT_LOAD_NO_AUTOHINT;
            if(font.config->rasterizer_flags & e_font_rasterizer_flags::force_auto_hint) font_flags |= FT_LOAD_FORCE_AUTOHINT;
            if(font.config->rasterizer_flags & e_font_rasterizer_flags::light_hinting) font_flags |= FT_LOAD_TARGET_LIGHT;
            else if(font.config->rasterizer_flags & e_font_rasterizer_flags::mono_hinting) font_flags |= FT_LOAD_TARGET_MONO;
            else font_flags |= FT_LOAD_TARGET_NORMAL;
            font_geometry.setFlags(font_flags);

            font_geometry.setEmbolden(font.config->rasterizer_flags & e_font_rasterizer_flags::bold);
            font_geometry.setOblique(font.config->rasterizer_flags & e_font_rasterizer_flags::oblique);

            font_geometry.loadCharset(ft_font, 1.0, loading_charset);
            font_geometry.loadMetrics(ft_font, 1.f);
            font_geometry.loadKerning(ft_font);

            void(*coloring_fn)(msdfgen::Shape&, double, unsigned long long) { };
            switch(font.config->coloring_mode) {
                case e_font_coloring_mode::simple: coloring_fn = &msdfgen::edgeColoringSimple; break;
                case e_font_coloring_mode::ink_trap: coloring_fn = &msdfgen::edgeColoringInkTrap; break;
                case e_font_coloring_mode::by_distance: coloring_fn = &msdfgen::edgeColoringByDistance; break;
            }

            if(coloring_fn) {
                for(msdf_atlas::GlyphGeometry& glyph : font.glyphs)
                    glyph.edgeColoring(coloring_fn, 3.f, 0);
            }

            const msdfgen::FontMetrics& metrics = font_geometry.getMetrics();
            c_font* fallback_font = font.config->fallback_font;
            fallback_font->metrics.size = font.config->size;
            fallback_font->metrics.ascender = metrics.ascenderY * font.config->size;
            fallback_font->metrics.descender = metrics.descenderY * font.config->size;
            fallback_font->metrics.line_height = metrics.lineHeight * font.config->size;
            fallback_font->metrics.px_range = font.config->pixel_range;

            for(auto& [keys, kerning] : font_geometry.getKerning()) {
                std::uint32_t left_codepoint = font_geometry.getGlyph(msdfgen::GlyphIndex(keys.first))->getCodepoint();
                std::uint32_t right_codepoint = font_geometry.getGlyph(msdfgen::GlyphIndex(keys.second))->getCodepoint();

                fallback_font->lookup_kerning(left_codepoint, right_codepoint) = kerning * font.config->size;
            }
            msdfgen::destroyFont(ft_font);
        }

        msdfgen::deinitializeFreetype(ft);
    }

    vec2_t<int> c_freetype_loader::pack_fonts(std::vector<font_data_t>& fonts) {
        std::vector<msdf_atlas::Rectangle> packing_rectangles{ };
        std::vector<msdf_atlas::GlyphGeometry*> packing_glyphs{ };

        for(font_data_t& font : fonts) {
            font_config_t* config = font.config;
            msdf_atlas::GlyphGeometry::GlyphAttributes attributes{ };
            attributes.scale = config->size;
            attributes.range = msdfgen::Range(config->pixel_range) / config->size;
            attributes.miterLimit = config->miter_limit;
            
            attributes.outerPadding = msdf_atlas::Padding(
                config->glyph_padding.min.x, config->glyph_padding.max.y,
                config->glyph_padding.max.x, config->glyph_padding.min.y
            ) / config->size;
            
            attributes.pxAlignOriginX = config->x_origin_align;
            attributes.pxAlignOriginY = config->y_origin_align;
            for(msdf_atlas::GlyphGeometry& glyph : font.glyphs) {
                if(glyph.isWhitespace()) continue;

                msdf_atlas::Rectangle rect{ };
                glyph.wrapBox(attributes);
                glyph.getBoxSize(rect.w, rect.h);
                if(rect.w > 0 && rect.h > 0) {
                    packing_rectangles.push_back(rect);
                    packing_glyphs.push_back(&glyph);
                }
            }
        }
        
        if(packing_rectangles.empty())
            return vec2_t<int>(0, 0);

        std::pair<int, int> dimensions = msdf_atlas::packRectangles<msdf_atlas::SquarePowerOfTwoSizeSelector>(packing_rectangles.data(), packing_rectangles.size(), 0);

        if(!(dimensions.first > 0 && dimensions.second > 0))
            return vec2_t<int>(0, 0);

        for(auto [glyph, rectangle] : std::views::zip(packing_glyphs, packing_rectangles))
            glyph->placeBox(rectangle.x, dimensions.second - (rectangle.y + rectangle.h));

        return vec2_t<int>(dimensions.first, dimensions.second);
    }

    void c_freetype_loader::rasterize_fonts(std::vector<font_data_t>& fonts, c_atlas::texture_t& atlas_texture) {
        for(font_data_t& font : fonts) {
            msdf_atlas::Workload([&font, &atlas_texture](int i, int) -> bool {
                const msdf_atlas::GlyphGeometry& glyph = font.glyphs[i];
                if(glyph.isWhitespace()) return true;

                int l{ }, b{ }, w{ }, h{ };
                glyph.getBoxRect(l, b, w, h);

                //@note: If artery_font is loaded, we try to get the already generated glyphs from its atlas,
                //       if this fails, then we simply generate them ourselves
                if(!font.config->artery_data.empty()) {
                    vec2_t<int> artery_offset{ }, artery_size = std::numeric_limits<float>::min();
                    if(artery_font::Glyph<float>* artery_glyph = font.artery_data.find_glyph(glyph.getCodepoint())) {
                        artery_offset.x = artery_glyph->imageBounds.l - 0.5f;
                        artery_offset.y = artery_glyph->imageBounds.b - 0.5f;

                        artery_size.x = (artery_glyph->imageBounds.r + 0.5f) - artery_offset.x;
                        artery_size.y = (artery_glyph->imageBounds.t + 0.5f) - artery_offset.y;
                    }

                    if(artery_size != std::numeric_limits<float>::min() && (artery_size.x != w || artery_size.y != h)) {
                        utils::logger(utils::e_log_type::warning, "\"{}\" ({}) glyph found in artery_font, but its dimensions do not match the packed glyph, perhaps the settings with which artery_font was generated differ from those specified in the config.", (char)glyph.getCodepoint(), glyph.getCodepoint());
                    } else {
                        for(auto [x, y] : std::views::cartesian_product(std::views::iota(0, w), std::views::iota(0, h))) {
                            float* channels = font.artery_data.image_read(artery_offset.x + x, artery_offset.y + y);

                            color_t<int> color{ };
                            switch(font.artery_data.image_channels()) {
                                //@note: monochrome, sdf, psdf
                                case 1: {
                                    std::uint8_t channel = msdfgen::pixelFloatToByte(*(float*)channels);
                                    color = color_t<int>(channel, channel);
                                } break;

                                //@note: msdf
                                case 3: {
                                    color = color_t<int>(
                                        msdfgen::pixelFloatToByte(channels[0]),
                                        msdfgen::pixelFloatToByte(channels[1]),
                                        msdfgen::pixelFloatToByte(channels[2]),
                                        255
                                    );
                                } break;

                                //@note: mtsdf
                                case 4: {
                                    color = color_t<int>(
                                        msdfgen::pixelFloatToByte(channels[0]),
                                        msdfgen::pixelFloatToByte(channels[1]),
                                        msdfgen::pixelFloatToByte(channels[2]),
                                        msdfgen::pixelFloatToByte(channels[3])
                                    );
                                } break;
                            }

                            atlas_texture.write_rgba(l + x, b + y, color);
                        }
                        return true;
                    }
                }

                switch(font.config->render_mode) {
                    case e_font_render_mode::monochrome:
                    case e_font_render_mode::sdf:
                    case e_font_render_mode::psdf: {
                        msdfgen::Bitmap<float, 1> glyph_bitmap(w, h);

                        if(font.config->render_mode == e_font_render_mode::monochrome) msdf_atlas::scanlineGenerator(glyph_bitmap, glyph, { });
                        if(font.config->render_mode == e_font_render_mode::sdf) msdf_atlas::sdfGenerator(glyph_bitmap, glyph, { });
                        if(font.config->render_mode == e_font_render_mode::psdf) msdf_atlas::psdfGenerator(glyph_bitmap, glyph, { });

                        for(auto [x, y] : std::views::cartesian_product(std::views::iota(0, w), std::views::iota(0, h))) {
                            const std::uint8_t channel = msdfgen::pixelFloatToByte(*glyph_bitmap(x, y));
                            atlas_texture.write_rgba(l + x, b + y, color_t<int>(channel, channel));
                        }
                    } break;

                    case e_font_render_mode::msdf: {
                        msdfgen::Bitmap<float, 3> glyph_bitmap(w, h);
                        msdf_atlas::msdfGenerator(glyph_bitmap, glyph, { });

                        for(auto [x, y] : std::views::cartesian_product(std::views::iota(0, w), std::views::iota(0, h))) {
                            const float* channels = glyph_bitmap(x, y);
                            atlas_texture.write_rgba(
                                l + x, b + y,
                                color_t<int>(msdfgen::pixelFloatToByte(channels[0]),
                                             msdfgen::pixelFloatToByte(channels[1]),
                                             msdfgen::pixelFloatToByte(channels[2]),
                                             255)
                            );
                        }
                    } break;

                    case e_font_render_mode::mtsdf: {
                        msdfgen::Bitmap<float, 4> glyph_bitmap(w, h);
                        msdf_atlas::mtsdfGenerator(glyph_bitmap, glyph, { });

                        for(auto [x, y] : std::views::cartesian_product(std::views::iota(0, w), std::views::iota(0, h))) {
                            const float* channels = glyph_bitmap(x, y);
                            atlas_texture.write_rgba(
                                l + x, b + y,
                                color_t<int>(msdfgen::pixelFloatToByte(channels[0]),
                                             msdfgen::pixelFloatToByte(channels[1]),
                                             msdfgen::pixelFloatToByte(channels[2]),
                                             msdfgen::pixelFloatToByte(channels[3]))
                            );
                        }
                    } break;
                }
                return true;
            }, font.glyphs.size()).finish(std::clamp(font.config->used_threads, 1, (int)std::thread::hardware_concurrency()));
        }
    }

    void c_freetype_loader::build_fonts(std::vector<font_data_t>& fonts) {
        for(font_data_t& font : fonts) {
            c_font* fallback_font = font.config->fallback_font;
            for(msdf_atlas::GlyphGeometry& glyph : font.glyphs) {
                rect_t<double> atlas_bounds{ };
                glyph.getQuadAtlasBounds(atlas_bounds.min.x, atlas_bounds.max.y, atlas_bounds.max.x, atlas_bounds.min.y);
                rect_t<float> uvs = rect_t<float>(atlas_bounds) / fallback_font->get_parent_atlas()->texture.size;

                rect_t<double> plane_bounds{ };
                glyph.getQuadPlaneBounds(plane_bounds.min.x, plane_bounds.max.y, plane_bounds.max.x, plane_bounds.min.y);
                plane_bounds *= font.config->size;

                glyph_t new_glyph{ };
                new_glyph.codepoint = glyph.getCodepoint();
                new_glyph.advance = glyph.getAdvance() * font.config->size;
                new_glyph.size = plane_bounds.size();
                if(new_glyph.size == 0.f)
                    new_glyph.visible = false;

                if(new_glyph.visible) {
                    new_glyph.upper_texture_uvs = rect_t<float>(uvs.min, uvs.origin({ 1.f, 0.f }));
                    new_glyph.lower_texture_uvs = rect_t<float>(uvs.origin({ 0.f, 1.f }), uvs.max);
                    new_glyph.bearing = plane_bounds.min;

                    new_glyph.rectangle.min = vec2_t<float>(new_glyph.bearing.x, fallback_font->metrics.ascender - new_glyph.bearing.y);
                    new_glyph.rectangle.max = new_glyph.rectangle.min + vec2_t<float>(new_glyph.size.x, new_glyph.size.y * -1.f);
                }
                fallback_font->add_glyph(new_glyph);
            }

            fallback_font->build_lookups();
        }
    }
}