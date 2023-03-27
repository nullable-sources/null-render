struct ps_input_t {
    float4 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

sampler texture_sampler;

float4 main(ps_input_t input) : COLOR {
    return input.color * tex2D(texture_sampler, input.uv);
}