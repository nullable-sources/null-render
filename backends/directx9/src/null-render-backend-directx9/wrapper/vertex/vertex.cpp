#include "vertex.h"

namespace ntl::render::directx9 {
    void c_vertex_shader::destroy() {
        if(empty()) return;
        if(auto result = shader->Release(); FAILED(result)) {
            sdk::logger(sdk::e_log_type::error, "cant release vertex shader, return code {}.", result);
            return;
        }
        shader = nullptr;
    }

    void c_vertex_shader::compile(const byte* source) {
        if(auto result = shared.device->CreateVertexShader((const DWORD*)source, &shader); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant create vertex shader, return code {}.", result);
    }

    void c_vertex_shader::set_constant(int location, const float* params, int count) {
        if(auto result = shared.device->SetVertexShaderConstantF(location, params, count); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant set vertex shader float constant, return code {}.", result);
    }

    void c_vertex_shader::set_constant(int location, const int* params, int count) {
        if(auto result = shared.device->SetVertexShaderConstantI(location, params, count); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant set vertex int constant, return code {}.", result);
    }

    void c_vertex_shader::set() {
        if(auto result = shared.device->SetVertexShader(shader); FAILED(result))
            sdk::logger(sdk::e_log_type::warning, "cant set vertex shader, return code {}.", result);
    }
}