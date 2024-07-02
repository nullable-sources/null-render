#include "sdf.hlsli"

struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;

float glyph_rounding : register(c7);
float outline_rounding : register(c8);

float4 main(ps_input_t input) : COLOR {
    float4 distances = tex2D(texture_sampler, input.uv);
    float msdf = median(distances.r, distances.g, distances.b);
    float sdf = distances.a;
    msdf = min(msdf, sdf + 0.1);

    return make_sdf(sdf, lerp(msdf, sdf, glyph_rounding), lerp(msdf, sdf, outline_rounding), input.color);
}
