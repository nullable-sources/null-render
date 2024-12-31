#include <micro-tess/planarize_division.h>
#include <micro-tess/stroke_tessellation.h>

#include "../../backend/shaders/passthrough.h"
#include "../../backend/state-pipeline/state-pipeline.h"

#include "draw-list.h"

namespace ntl::render {
    void c_draw_list::handle() {
        backend::state_pipeline->meshes.push(mesh);
        backend::state_pipeline->shaders.push(backend::color_shader);
        c_command_buffer::handle();
        backend::state_pipeline->meshes.pop();
        backend::state_pipeline->shaders.pop();
    }

    void c_draw_list::stroke_path(const path_t& path, const std::shared_ptr<c_brush>& brush) {
        std::shared_ptr<c_geometry_command> command = std::make_shared<c_geometry_command>(&mesh->geometry_buffer);
        command->topology = backend::e_topology::triangle_strip;

        microtess::dynamic_array<vec2_t<float>> vertecies{ };
        microtess::dynamic_array<backend::index_t> indices{ };
        microtess::triangles::indices indeces_type{ };

        microtess::stroke_line_join mt_line_join{ };
        switch(path.line_join) {
            case e_line_join::bevel: { mt_line_join = microtess::stroke_line_join::bevel; } break;
            case e_line_join::miter: { mt_line_join = microtess::stroke_line_join::miter; } break;
            case e_line_join::round: { mt_line_join = microtess::stroke_line_join::round; } break;
        }

        if(path.line_cap == e_line_cap::joint)
            sdk::logger(sdk::e_log_type::warning, "e_line_cap::joint using only for add_poly_line method");

        using triangulation = microtess::stroke_tessellation<float, decltype(vertecies), decltype(indices), microtess::dynamic_array<microtess::triangles::boundary_info>>;

        for(unsigned ix = 0; ix < path.paths_vertices.size(); ++ix) {
            auto chunk = path.paths_vertices[ix];
            const auto chunk_size = chunk.size();
            if(chunk_size == 0) continue;
            bool is_closing = chunk_size >= 3 && chunk[chunk_size - 3] == chunk[chunk_size - 1] && chunk[chunk_size - 3] == chunk[chunk_size - 2];

            triangulation::template compute_with_dashes(
                path.stroke_width, is_closing, (microtess::stroke_cap)path.line_cap, mt_line_join,
                path.miter_limit, path.dash_pattern, path.dash_offset,
                chunk.data(), chunk_size - (is_closing ? 2 : 0), vertecies, indices, indeces_type
            );
        }

        command->index_count += indices.size();
        std::ranges::move(indices, std::back_inserter(mesh->geometry_buffer.index_buffer));

        command->vertex_count += vertecies.size();
        for(const vec2_t<float>& current_point : vertecies) {
            mesh->geometry_buffer.add_vertex(current_point, { }, brush->color);
        }

        add_command(brush->prepare_command(std::move(command)));
    }

    void c_draw_list::fill_path(const path_t& path, const std::shared_ptr<c_brush>& brush) {
        std::shared_ptr<c_geometry_command> command = std::make_shared<c_geometry_command>(&mesh->geometry_buffer);

        microtess::dynamic_array<vec2_t<float>> vertecies{ };
        microtess::dynamic_array<backend::index_t> indecies{ };
        microtess::triangles::indices type{ };
        using triangulation = microtess::planarize_division<float, decltype(vertecies), decltype(indecies), microtess::dynamic_array<microtess::triangles::boundary_info>>;
        triangulation::template compute<path_t::chunker_t>(
            path.paths_vertices, (microtess::fill_rule)path.fill_rule, (microtess::tess_quality)path.quality,
            vertecies, type, indecies
        );

        command->index_count += indecies.size();
        std::ranges::move(indecies, std::back_inserter(mesh->geometry_buffer.index_buffer));

        command->vertex_count += vertecies.size();
        for(const vec2_t<float>& point : vertecies) {
            mesh->geometry_buffer.add_vertex(point, { }, brush->color);
        }

        add_command(brush->prepare_command(std::move(command)));
    }

    void c_draw_list::add_poly_line(const std::vector<vec2_t<float>>& points, const stroke_t& stroke, const std::shared_ptr<c_brush>& brush, const pen_t& pen) {
        if(points.size() < 2) return;
        std::shared_ptr<c_geometry_command> command = std::make_shared<c_geometry_command>(&mesh->geometry_buffer);

        const bool have_pen = stroke.line_join != e_line_join::none && pen.brush;
        std::vector<backend::index_t> outward_order{ }, inward_order{ };

        const float half_thickness = stroke.thickness / 2.f;
        const float outward_thickness = stroke.thickness * stroke.origin;
        const float inward_thickness = stroke.thickness * (1.f - stroke.origin);

        std::vector<stroke_t::segment_t> segments{ };
        stroke.build_segments(segments, points);
        for(const stroke_t::segment_t& segment : segments) {
            if(have_pen) {
                outward_order.push_back(command->vertex_count + segment.begin_edge->outward_begin);
                if(segment.begin_edge->outward_begin != segment.begin_edge->outward_end)
                    outward_order.push_back(command->vertex_count + segment.begin_edge->outward_end);

                inward_order.push_back(command->vertex_count + segment.begin_edge->inward_begin);
                if(segment.begin_edge->inward_begin != segment.begin_edge->inward_end)
                    inward_order.push_back(command->vertex_count + segment.begin_edge->inward_end);
            }

            if(!(stroke.line_cap != e_line_cap::joint && segment.is_last)) {
                const size_t next_edge_offset = segment.is_last && stroke.line_join != e_line_join::none ? 0u : command->vertex_count + segment.begin_edge->join_size;

                command->index_count += 6;
                mesh->geometry_buffer
                    .add_indexes(
                        command->vertex_count + segment.begin_edge->outward_end, next_edge_offset + segment.end_edge->outward_begin, next_edge_offset + segment.end_edge->inward_begin,
                        command->vertex_count + segment.begin_edge->outward_end, next_edge_offset + segment.end_edge->inward_begin, command->vertex_count + segment.begin_edge->inward_end
                    );
            }

            if(stroke.line_cap != e_line_cap::joint && (segment.is_first || segment.is_last)) {
                const vec2_t<float>& cap_direction = segment.is_first ? -segment.begin_edge->to_next_direction : segment.begin_edge->from_previous_direction;

                const vec2_t<float>& direction = segment.is_first ? segment.begin_edge->to_next_direction : segment.begin_edge->from_previous_direction;
                const vec2_t<float> outward_delta = math::invert_vector_axis(direction, math::e_rotation::ccw) * outward_thickness;
                const vec2_t<float> inward_delta = math::invert_vector_axis(direction, math::e_rotation::cw) * inward_thickness;

                const vec2_t<float> outward_vertex = *segment.begin_edge->point + outward_delta + cap_direction * (stroke.line_cap == e_line_cap::square ? outward_thickness : 0.f);
                const vec2_t<float> inward_vertex = *segment.begin_edge->point + inward_delta + cap_direction * (stroke.line_cap == e_line_cap::square ? outward_thickness : 0.f);

                command->vertex_count += 2;
                mesh->geometry_buffer
                    .add_vertex(outward_vertex, { }, brush->color)
                    .add_vertex(inward_vertex, { }, brush->color);
            } else {
                if(stroke.line_join == e_line_join::bevel) {
                    vec2_t<float> distance = segment.begin_edge->get_mitter_offset(half_thickness);
                    const math::e_rotation rotation = segment.begin_edge->inversed ? math::e_rotation::cw : math::e_rotation::ccw;
                    if(!segment.begin_edge->inversed)
                        distance *= -1.f;

                    vec2_t<float> origin_offset{ };
                    if(stroke.origin != 0.5f)
                        origin_offset = segment.begin_edge->get_mitter_offset(inward_thickness > outward_thickness ? (outward_thickness - half_thickness) : (half_thickness - inward_thickness));

                    command->index_count += 3;
                    mesh->geometry_buffer.add_indexes(command->vertex_count, command->vertex_count + 1, command->vertex_count + 2);

                    const vec2_t<float> point = *segment.begin_edge->point + origin_offset;
                    const vec2_t<float> miter_vertex = point + distance;
                    const vec2_t<float> to_previous_vertex = point + math::invert_vector_axis(segment.begin_edge->from_previous_direction, rotation) * half_thickness;
                    const vec2_t<float> to_next_vertex = point + math::invert_vector_axis(segment.begin_edge->to_next_direction, rotation) * half_thickness;

                    command->vertex_count += 3;
                    mesh->geometry_buffer
                        .add_vertex(miter_vertex, { }, brush->color)
                        .add_vertex(to_previous_vertex, { }, brush->color)
                        .add_vertex(to_next_vertex, { }, brush->color);
                } else if(stroke.line_join == e_line_join::miter) {
                    const vec2_t<float> outward_vertex = *segment.begin_edge->point + segment.begin_edge->get_mitter_offset(outward_thickness);
                    const vec2_t<float> inward_vertex = *segment.begin_edge->point - segment.begin_edge->get_mitter_offset(inward_thickness);

                    command->vertex_count += 2;
                    mesh->geometry_buffer
                        .add_vertex(outward_vertex, { }, brush->color)
                        .add_vertex(inward_vertex, { }, brush->color);
                } else {
                    const vec2_t<float> outward_vertex = *segment.begin_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::ccw) * outward_thickness;
                    const vec2_t<float> inward_vertex = *segment.begin_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::cw) * inward_thickness;

                    command->vertex_count += 2;
                    mesh->geometry_buffer
                        .add_vertex(outward_vertex, { }, brush->color)
                        .add_vertex(inward_vertex, { }, brush->color);
                }
            }

            //@note: in e_line_join::none each segment itself creates an end in addition to the beginning, the check is needed so that e_line_cap is displayed correctly
            if(stroke.line_join == e_line_join::none && ((!segment.is_last && *segment.end_edge->point != *std::prev(points.end())) || stroke.line_cap == e_line_cap::joint)) {
                const vec2_t<float> outward_vertex = *segment.end_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::ccw) * outward_thickness;
                const vec2_t<float> inward_vertex = *segment.end_edge->point + math::invert_vector_axis(segment.begin_edge->to_next_direction, math::e_rotation::cw) * inward_thickness;

                command->vertex_count += 2;
                mesh->geometry_buffer
                    .add_vertex(outward_vertex, { }, brush->color)
                    .add_vertex(inward_vertex, { }, brush->color);
            }
        }

        std::shared_ptr<i_command> pen_command{ };
        if(pen.brush) pen_command = std::move(pen.around_stroke(command, outward_order, inward_order, stroke.line_cap == e_line_cap::joint));
        if(pen.brush && pen.layer == e_pen_layer::background) add_command(std::move(pen_command));
        add_command(brush->prepare_command(std::move(command)));
        if(pen.brush && pen.layer == e_pen_layer::foreground) add_command(std::move(pen_command));
    }

    void c_draw_list::add_convex_shape(const std::vector<vec2_t<float>>& points, const std::shared_ptr<c_brush>& brush, const pen_t& pen) {
        if(points.size() < 3) return;
        std::shared_ptr<c_geometry_command> command = c_geometry_command::instance(&mesh->geometry_buffer);

        command->index_count += (points.size() - 2) * 3;
        for(int i = 2; i < points.size(); i++)
            mesh->geometry_buffer.add_indexes(0, i - 1, i);

        command->vertex_count += points.size();
        for(const vec2_t<float>& current_point : points) {
            mesh->geometry_buffer.add_vertex(current_point, { }, brush->color);
        }

        std::shared_ptr<i_command> pen_command{ };
        if(pen.brush) pen_command = std::move(pen.around_convex_shape(command));
        if(pen.brush && pen.layer == e_pen_layer::background) add_command(std::move(pen_command));
        add_command(brush->prepare_command(std::move(command)));
        if(pen.brush && pen.layer == e_pen_layer::foreground) add_command(std::move(pen_command));
    }

    void c_draw_list::compile() {
        mesh->compile();
    }

    void c_draw_list::clear() {
        c_command_buffer::clear();
        mesh->clear_geometry();
    }
}
