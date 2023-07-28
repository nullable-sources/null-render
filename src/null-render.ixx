module;
#include <type_traits>
#include <xstring>
#include <ranges>
#include <vector>
#include <cmath>
#include <ranges>
#include <mutex>
#include <future>
export module null.render;

export import :font;
export import :font.atlas;
export import :font.configs;
export import :font.glyph;
export import :font.freetype_loader;
export import :font.truetype_loader;

export import :graphic.path;

export import :graphic.filter;
export import :graphic.filters.linear_gradient;
export import :graphic.filters.quad_gradient;
export import :graphic.filters.sdf;
export import :graphic.filters.texture;

export import :graphic.command;
export import :graphic.commands.geometry;
export import :graphic.commands.renderer;

export import :graphic.draw_list;
export import :graphic.draw_list.pen;
export import :graphic.draw_list.stroke;
export import :graphic.draw_list.brush;
export import :graphic.draw_list.brushes.text;
export import :graphic.draw_list.brushes.sdf_text;

export import :backend;

export import :backend.renderer;
export import :backend.renderer.frame_buffer;
export import :backend.renderer.geometry_buffer;
export import :backend.renderer.mesh;
export import :backend.renderer.object;

export import :backend.shader;
export import :backend.shaders.passthrough;
export import :backend.shaders.linear_gradient;
export import :backend.shaders.quad_gradient;
export import :backend.shaders.sdf;