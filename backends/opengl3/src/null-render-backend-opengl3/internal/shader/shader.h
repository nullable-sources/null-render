#pragma once
#include "../../wrapper/fields.h"

namespace ntl::render::opengl3 {
    class c_shader : private virtual backend::i_shader {
    public:
        std::unique_ptr<c_program> program{ };
        c_fragment_shader* fragment_shader{ };
        c_vertex_shader* vertex_shader{ };

    public:
        c_shader(c_fragment_shader* _fragment_shader, c_vertex_shader* _vertex_shader) : fragment_shader(_fragment_shader), vertex_shader(_vertex_shader) { }
        virtual ~c_shader() { }

    public:
        virtual void create() override;
        virtual void destroy() override;

        virtual void use() override;

    public:
        virtual bool empty() const override { return !program || program->empty(); }
    };

    class c_default_shader : public c_shader {
    public: using c_shader::c_shader;
    public:
        c_uniform<matrix4x4_t> matrix{ };
        c_uniform<vec2_t<float>> translation{ };
        c_uniform<float> depth{ };

    public:
        virtual ~c_default_shader() { }

    public:
        virtual void create() override;

        virtual void use() override;
    };
}