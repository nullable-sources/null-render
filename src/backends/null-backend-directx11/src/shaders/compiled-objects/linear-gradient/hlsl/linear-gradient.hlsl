cbuffer constant_buffer : register(b0) {
    int4 colors[16];
    float4 stops[16]; //@note: чтобы понимать почему stops типа float4 см. реализацию шейдера на dx9
    float angle;
    int stops_count;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(ps_input_t input) : SV_Target {
    float2 uv = input.uv - 0.5f;
    float t = 0.5f + length(uv) * cos(atan2(-uv.y, uv.x) + angle);
    
    float4 color = colors[0] / 255.f;
    for (int i = 1; i < stops_count; ++i) {
        color = lerp(color, colors[i] / 255.f, smoothstep(stops[i - 1].x, stops[i].x, t));
    }
    return color;
}