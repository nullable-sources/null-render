#include "stencil-state-instances-stack.h"

namespace ntl::render::directx11 {
    ID3D11DepthStencilState* c_stencil_state_instances_stack::instance_object(std::size_t desc_hash, const dx_stencil_desc& desc) {
        ID3D11DepthStencilState** object = &instances[desc_hash];
        if(auto result = ntl::render::directx11::shared.device->CreateDepthStencilState(&desc, object); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "failed to create stencil state, error code: {}", result);
        return *object;
    }
}