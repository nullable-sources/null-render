struct ps_input_t {
    float4 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

sampler texture_sampler;

float aa                : register(c0);
float outline_thickness : register(c1);
float4 outline_start    : register(c2);
float4 outline_end      : register(c3);

float stretch(float low, float high, float x) {
    return clamp((x - low) / (high - low), 0.0, 1.0);
}

float4 main(ps_input_t input) : COLOR {
    float distance = tex2D(texture_sampler, input.uv).a;
    if(distance >= 0.5f + aa)  {
        return input.color;
    }
    
    if(distance <= outline_thickness - aa)
        discard;
    
    float m = stretch(0.5f - aa, min(1.f, 0.5f + aa), distance);
    float outer_mix = stretch(outline_thickness, 0.5f, distance);
    float4 outer = lerp(outline_end / 255.f, outline_start / 255.f, outer_mix);
    outer.a *= stretch(outline_thickness - aa, outline_thickness + aa, distance);
    float ia = m * input.color.a;
    float oa = (1.0 - m) * outer.a;
    float a = ia + oa;
    return float4((input.color.rgb * ia + outer.rgb * oa) / a, a);

}