#pragma once
#include "../../backend/internal/mesh.h"

#include "../commands/renderer/renderer.h"
#include "../brushes/sdf-text/sdf-text.h"
#include "../pen/pen.h"
#include "../path/path.h"

namespace null::render {
    class c_draw_list : public c_command_buffer {
    public:
        static std::shared_ptr<c_draw_list> instance(std::unique_ptr<backend::c_mesh>&& mesh) { return std::make_shared<c_draw_list>(std::move(mesh)); }

    public:
        std::unique_ptr<backend::c_mesh> mesh{ };

    public:
        c_draw_list(std::unique_ptr<backend::c_mesh>&& _mesh) : mesh(std::move(_mesh)) { }
        virtual ~c_draw_list() { if(mesh) mesh->destroy(); }

    public:
        virtual void handle() override;

    public:
        void flush() {
            compile();
            handle();
            clear();
        }

        void stroke_path(const path_t& path, const std::shared_ptr<c_brush>& brush);
        void fill_path(const path_t& path, const std::shared_ptr<c_brush>& brush);

        void add_poly_line(const std::vector<vec2_t<float>>& points, const stroke_t& stroke, const std::shared_ptr<c_brush>& brush, const pen_t& pen = { });
        void add_convex_shape(const std::vector<vec2_t<float>>& points, const std::shared_ptr<c_brush>& brush, const pen_t& pen = { });

        template <typename string_t> std::shared_ptr<i_command> make_text_command(const string_t& text, vec2_t<float> pos, const std::shared_ptr<i_text_brush>& text_brush, vec2_t<float>* out_size = nullptr) { return make_text_command(std::basic_string_view(text), pos, text_brush, out_size); }
        template <typename char_t> std::shared_ptr<i_command> make_text_command(std::basic_string_view<char_t> text, vec2_t<float> pos, const std::shared_ptr<i_text_brush>& text_brush, vec2_t<float>* out_size = nullptr);

        template <typename string_t> vec2_t<float> add_text(const string_t& text, vec2_t<float> pos, const std::shared_ptr<i_text_brush>& text_brush) { return add_text(std::basic_string_view(text), pos, text_brush); }
        template <typename char_t> vec2_t<float> add_text(std::basic_string_view<char_t> text, vec2_t<float> pos, const std::shared_ptr<i_text_brush>& text_brush);

        virtual void compile();
        virtual void clear() override;
    };

    inline std::shared_ptr<c_draw_list> draw_list{ };
}

#include "draw-list.inl"