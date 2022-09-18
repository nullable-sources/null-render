#include <null-renderer-directx11.h>

//@note: i'm too lazy to rewrite this now, so i'll do it later
namespace null::render::shaders {
	namespace sources {
		#include "pixel/compiled/pixel.h"
		static std::vector<byte> pixel_data() {
			return std::vector<byte>{ pixel, pixel + sizeof(pixel) };
		}

		#include "vertex/compiled/vertex.h"
		static std::vector<byte>  vertex_data() {
			return std::vector<byte>{ vertex, vertex + sizeof(vertex) };
		}
	}

	enum class e_shader_flags {
		dont_use_constant_buffer = 1 << 0
	}; enum_create_bit_operators(e_shader_flags, true);
	enum_create_cast_operator(e_shader_flags, -);

	template <typename shader_type>
	class i_shader {
	public:
		e_shader_flags flags{ };

		shader_type* shader{ };
		ID3D11Buffer* constant_buffer{ };

	public:
		i_shader() = default;
		i_shader(e_shader_flags _flags) : flags(_flags) { }
		i_shader(std::vector<byte> shader_source, size_t constant_size) { create(shader_source, constant_size); }

		virtual void create(std::vector<byte> shader_source, size_t constant_size = 0) {
			create_shader(shader_source);
			create_constant_buffer(constant_size);
		}

		virtual void release() {
			if(shader) { shader->Release(); shader = nullptr; }
			if(constant_buffer) { constant_buffer->Release(); constant_buffer = nullptr; }
		}

		virtual void create_shader(std::vector<byte> shader_source) { };
		virtual void create_constant_buffer(size_t constant_size) {
			if(flags & e_shader_flags::dont_use_constant_buffer || constant_buffer != nullptr || constant_size == 0) return;

			CD3D11_BUFFER_DESC desc(((constant_size + 15) / 16) * 16, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
			directx11::device->CreateBuffer(&desc, NULL, &constant_buffer);
		}

		virtual void set_shader() = 0;
		virtual void set_constant() = 0;

		template <typename constant_type>
		void edit_constant(constant_type constant) {
			D3D11_MAPPED_SUBRESOURCE mapped_subresource;
			directx11::context->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
			*(constant_type*)mapped_subresource.pData = constant;
			directx11::context->Unmap(constant_buffer, 0);
		}

		virtual operator bool() { return shader != nullptr && (constant_buffer != nullptr && !(flags & e_shader_flags::dont_use_constant_buffer)); }
		virtual bool operator!() { return shader == nullptr || (constant_buffer == nullptr && !(flags & e_shader_flags::dont_use_constant_buffer)); }
	};

	class c_vertex_shader : public i_shader<ID3D11VertexShader> {
	public:
		c_vertex_shader() = default;
		c_vertex_shader(e_shader_flags _flags) : i_shader(_flags) { }
		c_vertex_shader(std::vector<byte> shader_source, size_t constant_size) : i_shader(shader_source, constant_size) { }

		virtual void create_shader(std::vector<byte> shader_source) override {
			if(!shader)
				directx11::device->CreateVertexShader(shader_source.data(), shader_source.size(), 0, &shader);
		}

		virtual void set_shader() override {
			if(shader)
				directx11::context->VSSetShader(shader, nullptr, 0);
		}

		virtual void set_constant() override {
			if(constant_buffer)
				directx11::context->VSSetConstantBuffers(0, 1, &constant_buffer);
		}
	};

	class c_pixel_shader : public i_shader<ID3D11PixelShader> {
	public:
		c_pixel_shader() = default;
		c_pixel_shader(e_shader_flags _flags) : i_shader(_flags) { }
		c_pixel_shader(std::vector<byte> shader_source, size_t constant_size) : i_shader(shader_source, constant_size) { }

		virtual void create_shader(std::vector<byte> shader_source) override {
			if(!shader)
				directx11::device->CreatePixelShader(shader_source.data(), shader_source.size(), 0, &shader);
		}

		virtual void set_shader() override {
			if(!shader) throw std::runtime_error{ "shader == nullptr" };
			directx11::context->PSSetShader(shader, nullptr, 0);
		}

		virtual void set_constant() override {
			if(!constant_buffer) throw std::runtime_error{ "constant_buffer == nullptr" };
			directx11::context->PSSetConstantBuffers(0, 1, &constant_buffer);
		}
	};

	void setup_render_state(c_draw_list::draw_data_t* _draw_data);
	void create_shaders();
	void release_shaders();
	void win_proc();

	namespace pixel {
		inline c_pixel_shader shader{ e_shader_flags::dont_use_constant_buffer };

		static void create_shader_control() { if(!shader) shader.create(sources::pixel_data()); }
		static void release_shader_control() { if(shader) shader.release(); }
	}

	namespace vertex {
		struct constant { float matrix[4][4]; } inline constants;
		inline c_vertex_shader shader{ };

		static void create_shader_control() { if(!shader) shader.create(sources::vertex_data(), sizeof(constant)); }
		static void release_shader_control() { if(shader) shader.release(); }
	}
}