#include "sdf.hlsli"

cbuffer constant_buffer : register(b0) {
    int4 outline_color;
    float outline_blur, outline_width_relative, outline_width_absolute;
    float threshold, out_bias;
    float px_range;
    float glyph_rounding, outline_rounding;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source;

float4 main(ps_input_t input) : SV_Target {
    float4 distances = texture_source.Sample(texture_sampler, input.uv);
    float msdf = median(distances.r, distances.g, distances.b);
    float sdf = distances.a;
    msdf = min(msdf, sdf + 0.1);
    
    return make_sdf(sdf, lerp(msdf, sdf, glyph_rounding), lerp(msdf, sdf, outline_rounding), input.color,
                    outline_width_relative, outline_width_absolute, outline_blur, outline_color,
                    out_bias, threshold, px_range);
}
