struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float angle : register(c0);
float stops_count : register(c1);
float2 origin : register(c2);
float4 stops[16] : register(c3);
float4 colors[16] : register(c19);

float4 main(ps_input_t input) : COLOR {
    float2 uv = input.uv - origin;
    float _angle = angle + atan2(uv.x, -uv.y);
    float len = length(uv);
    float2 t = float2(cos(_angle) * len, sin(_angle) * len) + origin;

    float4 color = colors[0] / 255.f;
    color.rgb *= color.a;
    for(int i = 1; i < stops_count; ++i) {
        float4 premultiplied = colors[i] / 255.f;
        premultiplied.rgb *= premultiplied.a;
        color = lerp(color, premultiplied, smoothstep(stops[i - 1].x, stops[i].x, t.x));
    }
    return color;
}
