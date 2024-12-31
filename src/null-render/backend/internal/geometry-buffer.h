#pragma once
#include <null-sdk.h>

namespace ntl::render::backend {
    enum class e_topology { triangle_list, triangle_strip };

    using index_t = std::uint32_t;

    struct vertex_t {
    public:
        vertex_t(const vec2_t<float>& _pos) : pos(_pos) { }
        vertex_t(const vec2_t<float>& _pos, const vec2_t<float>& _uv, const color_t<int> _color)
            : pos(_pos), uv(_uv), color(_color) { }

    public:
        vec2_t<float> pos{ }, uv{ };
        color_t<int> color{ };
    };

    class c_geometry_buffer {
    public:
        std::vector<vertex_t> vertex_buffer{ };
        std::vector<index_t> index_buffer{ };

    public:
        template <typename self_t> inline auto&& add_index(this self_t&& self, index_t index) { self.index_buffer.emplace_back(index); return self; }

        template <typename self_t, std::convertible_to<index_t> ... args_t>
        inline auto&& add_indexes(this self_t&& self, args_t&& ... indexes) {
            std::ranges::move(std::initializer_list{ std::forward<args_t>(indexes)... }, std::back_inserter(self.index_buffer));
            return self;
        }

        template <typename self_t> inline auto&& add_vertex(this self_t&& self, const vertex_t& vertex) { self.vertex_buffer.emplace_back(vertex); return self; }
        template <typename self_t> inline auto&& add_vertex(this self_t&& self, const vec2_t<float>& pos, const vec2_t<float>& uv, const color_t<int>& color) { self.vertex_buffer.emplace_back(vertex_t(pos, uv, color)); return self; }

        template <typename self_t, std::convertible_to<vertex_t> ... args_t>
        inline auto&& add_vertexes(this self_t&& self, args_t&& ... vertexes) {
            std::ranges::move(std::initializer_list{ std::forward<args_t>(vertexes)... }, std::back_inserter(self.vertex_buffer));
            return self;
        }

    public:
        inline void clear() { vertex_buffer.clear(); index_buffer.clear(); }
    };
}