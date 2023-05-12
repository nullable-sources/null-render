struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;

float amount : register(c0);
float2 direction : register(c1);
float2 texel_size : register(c2);
float4 uv_limits : register(c3);

static const float offsets[3] = { 0.0f, 1.3846153846f, 3.2307692308f };
static const float weights[3] = { 0.2270270270f, 0.3162162162f, 0.0702702703f };

float2 clamp_uv(float2 uv) {
    return clamp(uv, uv_limits.xy, uv_limits.zw);
}

float4 main(ps_input_t input) : COLOR {
    float4 color = input.color * tex2D(texture_sampler, clamp_uv(input.uv));
    color.rgb *= weights[0];

    for (int i = 1; i < 3; ++i) {
        float2 offset = texel_size * (offsets[i] * amount) * direction;
        color.rgb += tex2D(texture_sampler, clamp_uv(input.uv - offset)).rgb * weights[i];
        color.rgb += tex2D(texture_sampler, clamp_uv(input.uv + offset)).rgb * weights[i];
    }
    
    return color;
}