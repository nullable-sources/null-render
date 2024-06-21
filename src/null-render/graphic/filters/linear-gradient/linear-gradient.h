#pragma once
#include "../../../backend/shaders/linear-gradient.h"
#include "../filter.h"

namespace null::render {
    class c_linear_gradient_filter : public i_filter {
    public:
        static std::shared_ptr<c_linear_gradient_filter> instance() { return std::make_shared<c_linear_gradient_filter>(); }
        static std::shared_ptr<c_linear_gradient_filter> instance(radians_t angle, const vec2_t<float>& origin, const std::vector<std::pair<color_t<int>, float>>& stops) { return std::make_shared<c_linear_gradient_filter>(angle, origin, stops); }

    public:
        backend::i_linear_gradient_shader::constants_t constants{ };

    public:
        c_linear_gradient_filter() { }
        c_linear_gradient_filter(radians_t angle, const vec2_t<float>& origin, const std::vector<std::pair<color_t<int>, float>>& stops)
            : constants(angle, origin, stops) { }
        virtual ~c_linear_gradient_filter() { }

    public:
        void set_angle(radians_t angle) { constants.angle = angle; }
        void set_origin(const vec2_t<float>& origin) { constants.origin = origin; }
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
            return std::shared_ptr<c_linear_gradient_filter>(new c_linear_gradient_filter(*this));
        }
    };
}