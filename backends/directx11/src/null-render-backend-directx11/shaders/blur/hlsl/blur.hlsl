cbuffer constant_buffer : register(b0) {
    float4 weights[24];
    float4 offsets[24];
    float4 uv_limits;
    float2 texel_size;
    float2 direction;
    int iterations;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source;

float2 clamp_uv(float2 uv) {
    return clamp(uv, uv_limits.xy, uv_limits.zw);
}

//https://geidav.wordpress.com/2013/03/05/hidden-hlsl-performance-hit-accessing-unpadded-arrays-in-constant-buffers/
float4 main(ps_input_t input) : SV_Target {
    float4 color = input.color * texture_source.Sample(texture_sampler, clamp_uv(input.uv));
    color *= weights[0][0];

    for(int i = 1; i < iterations; ++i) {
        int chunk = i >> 2;
        int index = i & 3;
        float weight = weights[chunk][index];

        float2 offset = texel_size * offsets[chunk][index] * direction;
        color += texture_source.Sample(texture_sampler, clamp_uv(input.uv - offset)) * weight;
        color += texture_source.Sample(texture_sampler, clamp_uv(input.uv + offset)) * weight;
    }

    return color;
}