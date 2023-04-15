//@credits: imgui version - 1.80 WIP (https://github.com/ocornut/imgui/tree/v1.80)
#pragma once
#include <font/loaders/freetype/freetype.h>
#include <font/loaders/truetype/truetype.h>
#include <null-sdk.h>

#include <graphic/draw-list/draw-list.h>

#include <graphic/draw-list/brushes/quad-gradient-brush/quad-gradient-brush.h>
#include <graphic/draw-list/text-styles/sdf-text-style/sdf-text-style.h>

namespace null::render {
    namespace shared {
        inline vec2_t<float> viewport{ };
        inline std::uint32_t msaa_quality{ 8 };

        inline int arc_fast_tessellation_multiplier{ 1 };

        inline std::vector<vec2_t<float>> arc_fast_vertex{ };
    }

    void initialize();

    void begin_frame(const utils::win::c_window& window);
    void end_frame();
}