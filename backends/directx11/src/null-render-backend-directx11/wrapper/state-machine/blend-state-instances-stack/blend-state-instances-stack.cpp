#include "blend-state-instances-stack.h"

namespace ntl::render::directx11 {
    ID3D11BlendState* c_blend_state_instances_stack::instance_object(std::size_t desc_hash, const dx_blend_factor_desc& desc) {
        ID3D11BlendState** object = &instances[desc_hash];
        if(auto result = ntl::render::directx11::shared.device->CreateBlendState(&desc, object); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "failed to create blend state, error code: {}", result);
        return *object;
    }
}