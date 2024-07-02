#include "sdf.hlsli"

struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;

float4 main(ps_input_t input) : COLOR {
    float3 msdf = tex2D(texture_sampler, input.uv).rgb;
    float sdf = median(msdf.r, msdf.g, msdf.b);
    return make_sdf(sdf, sdf, sdf, input.color);
}
