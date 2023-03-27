struct ps_input_t {
    float4 pos : POSITION0;
    float4 color : COLOR0;
};

float4 main(ps_input_t input) : COLOR {
    return float4(1.f, 1.f, 1.f, 1.f);
}