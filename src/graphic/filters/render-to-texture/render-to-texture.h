#pragma once
#include <graphic/filters/filters.h>

namespace null::render::filters {
	class c_render_to_texture : public i_filter {
	public: using i_filter::i_filter;
		virtual void handle() override;
	};
}