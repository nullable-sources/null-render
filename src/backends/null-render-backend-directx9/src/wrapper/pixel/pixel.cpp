#include "wrapper/pixel/pixel.h"

namespace null::render::directx9 {
	void c_pixel_shader::compile(const byte* source) {
		if(auto result{ shared.device->CreatePixelShader((const DWORD*)source, &shader) }; FAILED(result))
			utils::logger(utils::e_log_type::warning, "cant create pixel shader, return code {}.", result);
	}

	void c_pixel_shader::destroy() {
		if(!empty()) {
			if(auto result{ shader->Release() }; FAILED(result))
				utils::logger(utils::e_log_type::warning, "cant release pixel shader, return code {}.", result);
			shader = nullptr;
		}
	}

	void c_pixel_shader::set_constant(int location, const float* params, int count) {
		if(auto result{ shared.device->SetPixelShaderConstantF(location, params, count) }; FAILED(result))
			utils::logger(utils::e_log_type::warning, "cant set pixel shader float constant, return code {}.", result);
	}

	void c_pixel_shader::set_constant(int location, const int* params, int count) {
		if(auto result{ shared.device->SetPixelShaderConstantI(location, params, count) }; FAILED(result))
			utils::logger(utils::e_log_type::warning, "cant set pixel int constant, return code {}.", result);
	}

	void c_pixel_shader::set() {
		if(auto result{ shared.device->SetPixelShader(shader) }; FAILED(result))
			utils::logger(utils::e_log_type::warning, "cant set pixel shader, return code {}.", result);
	}
}