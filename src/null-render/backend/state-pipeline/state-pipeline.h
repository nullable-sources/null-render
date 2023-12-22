#pragma once
#include "../internal/mesh.h"
#include "../internal/frame-buffer.h"
#include "../internal/shader.h"

namespace null::render::backend {
	class i_state_pipeline {
	public:
		template <typename object_t>
		class i_pipeline {
		public:
			std::vector<object_t*> stack{ };

		private:
			virtual void append() = 0;
			virtual void set_default() = 0;

		public:
			template <typename new_object_t> requires std::is_base_of_v<object_t, new_object_t> || std::is_same_v<object_t, new_object_t>
			void push(const std::unique_ptr<new_object_t>& object) { push(object.get()); }

			template <typename new_object_t> requires std::is_base_of_v<object_t, new_object_t> || std::is_same_v<object_t, new_object_t>
			void push(new_object_t* object) {
				stack.push_back(object);
				append();
			}

			void pop() {
				if(empty()) {
					set_default();
					return;
				}

				stack.pop_back();
				append_last();
			}

			void append_last() {
				if(empty()) set_default();
				else append();
			}

		public:
			bool empty() const { return stack.empty(); }
		};

		class c_framebuffers_pipeline : public i_pipeline<i_frame_buffer> {
		public:
			void append() override;
			void set_default() override;
		} framebuffers{ };

		class c_shaders_pipeline : public i_pipeline<i_shader> {
		public:
			void append() override;
			void set_default() override;
		} shaders{ };

		class c_meshes_pipeline : public i_pipeline<i_mesh> {
		public:
			void append() override;
			void set_default() override;
		} meshes{ };

		class c_textures_pipeline : public i_pipeline<void> {
		public:
			void append() override;
			void set_default() override;
		} textures{ };

	public:
		virtual void setup_state() = 0;
		virtual void save_state() = 0;
		virtual void restore_state() = 0;

	public:
		virtual void restore_framebuffer() = 0;
		virtual void restore_shader() = 0;
		virtual void restore_mesh() = 0;
		virtual void restore_texture() = 0;
	}; inline std::unique_ptr<i_state_pipeline> state_pipeline{ };
}
