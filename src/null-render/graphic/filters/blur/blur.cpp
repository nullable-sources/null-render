#include "../../../backend/post-processing/post-processing.h"
#include "../../../backend/renderer/renderer.h"
#include "../../../backend/shaders/passthrough.h"
#include "../../../backend/shaders/blur.h"
#include "../../../backend/state-pipeline/state-pipeline.h"

#include "blur.h"

namespace null::render {
	void c_blur_filter::make_constants() {
		//@credits: https://github.com/T0b1-iOS/draw_manager/blob/b6f0de06567598aa899237f23bff3fbdc6851723/impl/d3d11_manager.cpp#L304C6-L304C6
		static const auto gauss = [](float sigma, float x) {
			constexpr auto double_pi = 2 * std::numbers::pi;
			return 1. / (std::sqrt(double_pi) * sigma) * std::exp(-0.5 * (std::pow(x, 2) / std::pow(sigma, 2)));
		};

		constants.iterations = std::min(strength, 96);

		constants.weights[0] = gauss(sigma, 0);
		float sum = constants.weights[0];
		for(int i = 1; i < constants.iterations; i++) {
			int x_next = i + 1;
			const auto weight = gauss(sigma, i);
			const auto weight_next = gauss(sigma, x_next);
			const auto combined_weight = weight + weight_next;
			constants.weights[i] = combined_weight;
			constants.offsets[i] = (i * weight + x_next * weight_next) / combined_weight;

			sum += combined_weight * 2.f;
		}

		if(constants.iterations & 1) {
			constants.weights[constants.iterations] = gauss(sigma, constants.iterations);
			constants.offsets[constants.iterations] = constants.iterations;
			sum += constants.weights[constants.iterations] * 2;
		}

		std::ranges::transform(constants.weights, constants.weights.begin(), std::bind(std::divides<float>(), std::placeholders::_1, sum));

		constants.texel_size = vec2_t<float>(1.f) / shared::viewport;
		constants.uv_limits = (geometry_region + rect_t<float>(vec2_t<float>(0.5f), vec2_t<float>(-0.5f))) / shared::viewport;
	}

	void c_blur_filter::make_geometry(backend::c_geometry_buffer* geometry_buffer) {
		geometry_command = c_geometry_command::instance(geometry_buffer);

		backend::post_processing->generate_draw_geometry(
			geometry_command.get(),
			backend::post_processing->prepare_viewport_region(geometry_region),
			geometry_region / shared::viewport
		);
	}

	void c_blur_filter::set_child_command(std::shared_ptr<c_geometry_command>& _child_command) {
		geometry_region = region;
		if(region == -1.f)
			_child_command->get_bounding_box(geometry_region.min, geometry_region.max);

		make_geometry(_child_command->geometry_buffer);

		rect_t<float> uvs(vec2_t<float>(0.f), shared::viewport);
		if(backend::renderer->framebuffer_uvs_flipped()) {
			std::swap(uvs.min.y, uvs.max.y);
		}

		_child_command->recalculate_uvs_unbound(uvs.min, uvs.max);

		i_filter::set_child_command(_child_command);
	}

	void c_blur_filter::handle() {
		backend::i_frame_buffer* main_buffer = backend::post_processing->at(0);
		backend::i_frame_buffer* secondary_buffer = backend::post_processing->at(1);

		if(sources & e_blur_filter_sources::backbuffer)
			main_buffer->copy_from(backend::rendering_buffer.get());

		if(sources & e_blur_filter_sources::msaa) {
			if(sources & e_blur_filter_sources::backbuffer) {
				main_buffer->use();
				backend::post_processing->blit_buffer(backend::msaa_buffer.get());
			} else {
				main_buffer->copy_from(backend::msaa_buffer.get());
			}
		}

		if(downsample_count > 0) {
			rect_t<float> blit_region = geometry_region;
			rect_t<float> previous_blit{ };
			for(int i : std::views::iota(0, downsample_count + 1)) {
				previous_blit = blit_region;
				blit_region *= 0.5f;
				const bool break_downsample = blit_region.size() < 1.f;

				if(i == downsample_count || break_downsample)
					blit_region = geometry_region;

				std::swap(main_buffer, secondary_buffer);
				main_buffer->use();
				main_buffer->clear();

				backend::post_processing->blit_buffer_region(
					secondary_buffer,
					blit_region,
					previous_blit
				);

				if(break_downsample)
					break;
			}
		}

		secondary_buffer->clear();

		vec2_t<float> direction(1.f, 0.f);
		make_constants();
		backend::blur_shader->set_constants(constants);
		backend::blur_shader->use();
		for(int i : std::views::iota(0, pass_count)) {
			secondary_buffer->use();
			backend::renderer->set_texture(main_buffer->get_texture());
			backend::blur_shader->set_direction(direction);
			geometry_command->handle();

			std::swap(direction.x, direction.y);

			main_buffer->use();
			backend::renderer->set_texture(secondary_buffer->get_texture());
			backend::blur_shader->set_direction(direction);
			geometry_command->handle();
		}

		//@note: we set shader, framebuffers and textures when creating a blur,
		//		 because the child command for rendering does not change the state, we can do without push/pop
		backend::state_pipeline->framebuffers.append_last();
		backend::state_pipeline->textures.append_last();
		backend::state_pipeline->shaders.append_last();

		backend::state_pipeline->shaders.push(backend::texture_shader);
		backend::state_pipeline->textures.push(main_buffer->get_texture());
		child_command->handle();
		backend::state_pipeline->textures.pop();
		backend::state_pipeline->shaders.pop();
	}
}
