struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(ps_input_t input) : COLOR {
    return input.color;
}