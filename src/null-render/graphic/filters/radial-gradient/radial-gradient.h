#pragma once
#include "../../../backend/shaders/radial-gradient.h"
#include "../filter.h"

namespace ntl::render {
    class c_radial_gradient_filter : public i_filter {
    public:
        static std::shared_ptr<c_radial_gradient_filter> instance() { return std::make_shared<c_radial_gradient_filter>(); }
        static std::shared_ptr<c_radial_gradient_filter> instance(const vec2_t<float>& origin, const vec2_t<float>& radius, const std::vector<std::pair<color_t<int>, float>>& stops) { return std::make_shared<c_radial_gradient_filter>(origin, radius, stops); }

    public:
        backend::i_radial_gradient_shader::constants_t constants{ };

    public:
        c_radial_gradient_filter() { }
        c_radial_gradient_filter(const vec2_t<float>& origin, const vec2_t<float>& radius, const std::vector<std::pair<color_t<int>, float>>& stops)
            : constants(origin, radius, stops) { }
        virtual ~c_radial_gradient_filter() { }

    public:
        void set_origin(const vec2_t<float>& origin) { constants.origin = origin; }
        void set_radius(const vec2_t<float>& radius) { constants.radius = radius; }
        void set_stops(const std::vector<std::pair<color_t<int>, float>>& stops) { constants.stops = stops; }
        void add_stop(const color_t<int>& color, float uv) { constants.stops.push_back({ color, uv }); }

    public:
        virtual void set_child_command(std::shared_ptr<c_geometry_command>& _child_command) override {
            _child_command->set_default_uvs();
            i_filter::set_child_command(_child_command);
        }

        virtual void handle() override;

    public:
        virtual std::shared_ptr<i_filter> clone() const override {
            return std::shared_ptr<c_radial_gradient_filter>(new c_radial_gradient_filter(*this));
        }
    };
}