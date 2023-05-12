#pragma once
#include <graphic/draw-list/brushes/brush.h>

namespace null::render {
	struct texture_brush_t : public brush_t {
	public:
		rect_t<float> uvs{ };
		void* texture{ };

	public:
		template <typename self_t> auto&& set_texture(this self_t&& self, void* texture) { self.texture = texture; return self; }
		template <typename self_t> auto&& set_uvs(this self_t&& self, const rect_t<float>& uvs) { self.uvs = uvs; return self; }

	public:
		virtual std::unique_ptr<commands::i_command> prepare_command(std::unique_ptr<commands::c_geometry>& command) const override;
	};
}