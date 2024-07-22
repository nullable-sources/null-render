#pragma once
#include "../commands.h"
#include "../../../backend/internal/stencil-buffer.h"

namespace null::render {
    class c_texture_command : public i_command {
    public:
        static std::shared_ptr<c_texture_command> instance() { return std::make_shared<c_texture_command>(); }
        static std::shared_ptr<c_texture_command> instance(void* _texture) { return std::make_shared<c_texture_command>(_texture); }

    public:
        void* texture{ };

    public:
        c_texture_command() { }
        c_texture_command(void* _texture) : texture(_texture) { }

    public:
        void handle() override;
    };

    class c_clip_command : public i_command {
    public:
        static std::shared_ptr<c_clip_command> instance() { return std::make_shared<c_clip_command>(); }
        static std::shared_ptr<c_clip_command> instance(const rect_t<float>& _rect) { return std::make_shared<c_clip_command>(_rect); }

    public:
        rect_t<float> rect{ };

    public:
        c_clip_command() { }
        c_clip_command(const rect_t<float>& _rect) : rect(_rect) { }

    public:
        void handle() override;
    };

    class c_set_matrix_command : public i_command {
    public:
        static std::shared_ptr<c_set_matrix_command> instance() { return std::make_shared<c_set_matrix_command>(); }
        static std::shared_ptr<c_set_matrix_command> instance(const matrix4x4_t& _matrix) { return std::make_shared<c_set_matrix_command>(_matrix); }

    public:
        matrix4x4_t matrix{ };

    public:
        c_set_matrix_command() { }
        c_set_matrix_command(const matrix4x4_t& _matrix) : matrix(_matrix) { }

    public:
        void handle() override;
    };

    class c_update_matrix_command : public i_command {
    public:
        static std::shared_ptr<c_update_matrix_command> instance() { return std::make_shared<c_update_matrix_command>(); }
        static std::shared_ptr<c_update_matrix_command> instance(const matrix4x4_t& _matrix) { return std::make_shared<c_update_matrix_command>(_matrix); }

    public:
        matrix4x4_t matrix{ };

    public:
        c_update_matrix_command() { }
        c_update_matrix_command(const matrix4x4_t& _matrix) : matrix(_matrix) { }

    public:
        void handle() override;
    };

    class c_set_translation_command : public i_command {
    public:
        static std::shared_ptr<c_set_translation_command> instance() { return std::make_shared<c_set_translation_command>(); }
        static std::shared_ptr<c_set_translation_command> instance(const vec2_t<float>& translation) { return std::make_shared<c_set_translation_command>(translation); }

    public:
        vec2_t<float> translation{ };

    public:
        c_set_translation_command() { }
        c_set_translation_command(const vec2_t<float>& _translation) : translation(_translation) { }

    public:
        void handle() override;
    };

    class c_update_translation_command : public i_command {
    public:
        static std::shared_ptr<c_update_translation_command> instance() { return std::make_shared<c_update_translation_command>(); }
        static std::shared_ptr<c_update_translation_command> instance(const vec2_t<float>& translation) { return std::make_shared<c_update_translation_command>(translation); }

    public:
        vec2_t<float> translation{ };

    public:
        c_update_translation_command() { }
        c_update_translation_command(const vec2_t<float>& _translation) : translation(_translation) { }

    public:
        void handle() override;
    };

    class c_stencil_test_command : public i_command {
    public:
        static std::shared_ptr<c_stencil_test_command> instance() { return std::make_shared<c_stencil_test_command>(); }
        static std::shared_ptr<c_stencil_test_command> instance(bool test) { return std::make_shared<c_stencil_test_command>(test); }

    public:
        bool test{ };

    public:
        c_stencil_test_command() { }
        c_stencil_test_command(bool _test) : test(_test) { }

    public:
        void handle() override;
    };

    class c_stencil_operation_command : public i_command {
    public:
        static std::shared_ptr<c_stencil_operation_command> instance() { return std::make_shared<c_stencil_operation_command>(); }
        static std::shared_ptr<c_stencil_operation_command> instance(backend::e_stencil_operation operation) { return std::make_shared<c_stencil_operation_command>(operation); }

    public:
        backend::e_stencil_operation operation{ };

    public:
        c_stencil_operation_command() { }
        c_stencil_operation_command(backend::e_stencil_operation _operation) : operation(_operation) { }

    public:
        void handle() override;
    };

    class c_stencil_clear_command : public i_command {
    public:
        static std::shared_ptr<c_stencil_clear_command> instance() { return std::make_shared<c_stencil_clear_command>(); }

    public:
        c_stencil_clear_command() { }

    public:
        void handle() override;
    };
}