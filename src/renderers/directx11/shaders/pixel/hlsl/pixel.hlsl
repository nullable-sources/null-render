struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};

sampler tex_sampler;
Texture2D tex_source;

float4 main(PS_INPUT input) : SV_Target {
    return input.col * tex_source.Sample(tex_sampler, input.uv);
}