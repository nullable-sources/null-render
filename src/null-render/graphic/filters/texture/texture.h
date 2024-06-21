#pragma once
#include "../filter.h"

namespace null::render {
    class c_texture_filter : public i_filter {
    public:
        static std::shared_ptr<c_texture_filter> instance() { return std::make_shared<c_texture_filter>(); }
        static std::shared_ptr<c_texture_filter> instance(void* texture, const rect_t<float>& uvs) { return std::make_shared<c_texture_filter>(texture, uvs); }

    public:
        rect_t<float> uvs{ std::numeric_limits<float>::max() };
        void* texture{ };

    public:
        c_texture_filter() { }
        c_texture_filter(void* _texture, const rect_t<float>& _uvs) : texture(_texture), uvs(_uvs) { }
        virtual ~c_texture_filter() { }

    public:
        void set_texture(void* _texture) { texture = _texture; }
        void set_uvs(const rect_t<float>& _uvs) { uvs = _uvs; }

    public:
        virtual void set_child_command(std::shared_ptr<c_geometry_command>& _child_command) override {
            if(uvs != std::numeric_limits<float>::max()) _child_command->set_uvs(uvs);
            i_filter::set_child_command(_child_command);
        }

        virtual void handle() override;

    public:
        virtual std::shared_ptr<i_filter> clone() const override {
            return std::shared_ptr<c_texture_filter>(new c_texture_filter(*this));
        }
    };
}