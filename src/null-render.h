//@credits: imgui version - 1.80 WIP (https://github.com/ocornut/imgui/tree/v1.80)
#pragma once
#include "null-render/graphic/filters/linear-gradient/linear-gradient.h"
#include "null-render/graphic/filters/radial-gradient/radial-gradient.h"
#include "null-render/graphic/filters/blur/blur.h"
#include "null-render/graphic/filters/quad-gradient/quad-gradient.h"
#include "null-render/graphic/filters/sdf/sdf.h"

#include "null-render/font/loaders/freetype/freetype.h"
#include "null-render/graphic/commands/state-pipeline/state-pipeline.h"
#include "null-render/graphic/commands/renderer/renderer.h"
#include "null-render/graphic/draw-list/draw-list.h"
#include "null-render/graphic/path/path.h"
#include "null-render/backend/backend.h"

namespace ntl::render {
    void initialize(const vec2_t<int>& viewport);

    void begin_frame();
    void end_frame();
}