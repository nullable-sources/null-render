#pragma once
#include <graphic/filters/filters.h>

namespace null::render::filters {
	class c_blur : public i_filter {
	private:
		std::unique_ptr<commands::c_geometry> geometry_pass{ };

	public:
		rect_t<float> region{ };
		float amount{ };

	public:
		c_blur(std::unique_ptr<i_command>&& _child_command, const rect_t<float>& _region, const float& _amount)
			: i_filter{ std::move(_child_command) }, region{ _region }, amount{ _amount } { make_geometry(); }

	private:
		void make_geometry();

	public:
		virtual void handle() override;
	};
}