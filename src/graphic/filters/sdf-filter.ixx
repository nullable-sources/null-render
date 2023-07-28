module;
#include <memory>
#include <algorithm>
#include <cmath>
#include <vector>
#include <ranges>
#include <future>

export module null.render:graphic.filters.sdf;

import null.sdk;

import :graphic.filters.texture;
import :backend.renderer;
import :backend.shaders.sdf;
import :backend.shaders.passthrough;
import :font.truetype_loader;

export namespace null::render {
	class c_sdf_filter : public c_texture_filter {
	public:
		float size{ }, thickness{ };
		color_t<int> outline_start{ }, outline_end{ };

	public:
		c_sdf_filter(std::unique_ptr<i_command>&& _child_command, void* _texture, const float& _thickness, const float& _size, const color_t<int>& _outline_start, const color_t<int>& _outline_end)
			: c_texture_filter{ std::move(_child_command), _texture }, thickness{ _thickness }, size{ _size }, outline_start{ _outline_start }, outline_end{ _outline_end } { }

	public:
		virtual void handle() override {
			float aa{ 0.25f / c_truetype_loader::sdf_padding * float(c_truetype_loader::sdf_pixel_size) / size };
			if(size < 16.f) aa *= std::pow(size / 16.f, 2);

			backend::renderer->set_texture(texture);
			backend::shaders::sdf->set_aa(aa);
			backend::shaders::sdf->set_outline_thickness(std::clamp(0.5f - thickness / 2.0f, aa, 0.5f));
			backend::shaders::sdf->set_outline_start(outline_start);
			backend::shaders::sdf->set_outline_end(outline_end);
			backend::shaders::sdf->use();

			child_command->handle();

			backend::shaders::passthrough_color->use();
			backend::renderer->set_texture(nullptr);
		}
	};
}