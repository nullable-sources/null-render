#pragma once
#include "../../font.h"
#include "../loader.h"

namespace ntl::render {
    class c_freetype_loader : public i_font_loader {
    private:
        struct font_data_t;

    public:
        void build(c_atlas* atlas) override;

    private:
        void initialize_fonts(std::vector<font_data_t>& fonts);
        vec2_t<int> pack_fonts(std::vector<font_data_t>& fonts);
        void rasterize_fonts(std::vector<font_data_t>& fonts, c_atlas::texture_t& atlas_texture);
        void build_fonts(std::vector<font_data_t>& fonts);
    };
}