cbuffer constant_buffer : register(b0) {
    float4x4 projection_matrix;
};

struct vs_input_t {
    float2 position : POSITION;
    float2 uv : TEXCOORD0;
    int4 color : COLOR0;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

ps_input_t main(vs_input_t input) {
    ps_input_t output;
    output.position = mul(projection_matrix, float4(input.position, 0.f, 1.f));
    output.color = input.color / 255.f;
    output.uv = input.uv;
    return output;
}