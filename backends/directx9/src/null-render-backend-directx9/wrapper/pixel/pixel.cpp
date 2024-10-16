#include "pixel.h"

namespace ntl::render::directx9 {
    void c_pixel_shader::destroy() {
        if(empty()) return;
        if(auto result = shader->Release(); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "cant release pixel shader, return code {}.", result);
            return;
        }
        shader = nullptr;
    }

    void c_pixel_shader::compile(const byte* source) {
        if(auto result = shared.device->CreatePixelShader((const DWORD*)source, &shader); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant create pixel shader, return code {}.", result);
    }

    void c_pixel_shader::set_constant(int location, const float* params, int count) {
        if(auto result = shared.device->SetPixelShaderConstantF(location, params, count); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant set pixel shader float constant, return code {}.", result);
    }

    void c_pixel_shader::set_constant(int location, const int* params, int count) {
        if(auto result = shared.device->SetPixelShaderConstantI(location, params, count); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant set pixel int constant, return code {}.", result);
    }

    void c_pixel_shader::set() {
        if(auto result = shared.device->SetPixelShader(shader); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant set pixel shader, return code {}.", result);
    }
}