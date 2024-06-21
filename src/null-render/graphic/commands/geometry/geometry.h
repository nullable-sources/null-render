#pragma once
#include "../../../backend/internal/geometry-buffer.h"
#include "../commands.h"

namespace null::render {
    class i_geometry_command : public i_command {
    public:
        backend::e_topology topology{ };
        size_t index_offset{ }, vertex_offset{ };
        size_t index_count{ }, vertex_count{ };

    public:
        i_geometry_command() { }
        i_geometry_command(size_t _index_offset, size_t _vertex_offset)
            : index_offset(_index_offset), vertex_offset(_vertex_offset) { }
        i_geometry_command(size_t _index_offset, size_t _vertex_offset, size_t _index_count, size_t _vertex_count)
            : index_offset(_index_offset), vertex_offset(_vertex_offset), index_count(_index_count), vertex_count(_vertex_count) { }
        virtual ~i_geometry_command() { }

    public:
        virtual void handle() override;
    };

    class c_geometry_command : public i_geometry_command {
    public:
        static std::shared_ptr<c_geometry_command> instance(backend::c_geometry_buffer* _geometry_buffer) { return std::make_shared<c_geometry_command>(_geometry_buffer); }

    public:
        backend::c_geometry_buffer* geometry_buffer{ };

    public:
        c_geometry_command(backend::c_geometry_buffer* _geometry_buffer);
        virtual ~c_geometry_command() { }

    public:
        void get_bounding_box(vec2_t<float>& min, vec2_t<float>& max) const;
        void recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max) const;
        void recalculate_uvs_unbound(const vec2_t<float>& min, const vec2_t<float>& max) const;

        void recalculate_uvs(const vec2_t<float>& min, const vec2_t<float>& max, const rect_t<float>& uvs) const;

        void set_uvs(const rect_t<float>& uvs) const;
        void set_default_uvs() const;

        void clear_geometry();
    };
}