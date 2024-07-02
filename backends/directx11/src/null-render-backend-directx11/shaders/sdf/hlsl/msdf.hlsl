#include "sdf.hlsli"

cbuffer constant_buffer : register(b0) {
    int4 outline_color;
    float outline_blur, outline_width_relative, outline_width_absolute;
    float threshold, out_bias;
    float px_range;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source;

float4 main(ps_input_t input) : SV_Target {
    float3 msdf = texture_source.Sample(texture_sampler, input.uv).rgb;
    float sdf = median(msdf.r, msdf.g, msdf.b);
    return make_sdf(sdf, sdf, sdf, input.color,
                    outline_width_relative, outline_width_absolute, outline_blur, outline_color,
                    out_bias, threshold, px_range);
}
