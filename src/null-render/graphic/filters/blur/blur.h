#pragma once
#include "../../../backend/shaders/blur.h"
#include "../filter.h"

namespace null::render {
	enum class e_blur_filter_sources {
		none = 0,
		backbuffer = 1 << 0, //@note: this flag is only needed if the msaa buffer is not the primary one.
		msaa = 1 << 1,
	}; enum_create_bit_operators(e_blur_filter_sources, true);
	enum_create_cast_operator(e_blur_filter_sources, -);

	class c_blur_filter : public i_filter {
	public:
		static std::shared_ptr<c_blur_filter> instance() { return std::make_shared<c_blur_filter>(); }

	private:
		rect_t<float> geometry_region{ };

	public:
		e_blur_filter_sources sources{ e_blur_filter_sources::msaa };

		int strength{ 1 };
		float sigma{ 1.f };
		rect_t<float> region{ -1.f }; //@note: if region == -1, then the geometry region of the child command will be set, region is responsible for the area that will be blurred
		int downsample_count{ 0 };
		int pass_count{ 1 }; //@note: a value > 1 will increase the transparency of the original image

		backend::i_blur_shader::constants_t constants{ };
		std::shared_ptr<c_geometry_command> geometry_command{ };

	public:
		c_blur_filter() { }
		virtual ~c_blur_filter() { }

	public:
		void set_sources(e_blur_filter_sources _sources) { sources = _sources; }
		void set_strength(int _strength) { strength = _strength; }
		void set_sigma(float _sigma) { sigma = _sigma; }
		void set_region(const rect_t<float>& _region) { region = _region; }
		void set_downsample_count(int _downsample_count) { downsample_count = _downsample_count; }
		void set_pass_count(int _pass_count) { pass_count = _pass_count; }

	private:
		void make_constants();
		void make_geometry(backend::c_geometry_buffer* geometry_buffer);

	public:
		virtual void set_child_command(std::shared_ptr<c_geometry_command>& _child_command) override;
		virtual void handle() override;

	public:
		virtual std::shared_ptr<i_filter> clone() const override {
			return std::shared_ptr<c_blur_filter>(new c_blur_filter(*this));
		}
	};
}