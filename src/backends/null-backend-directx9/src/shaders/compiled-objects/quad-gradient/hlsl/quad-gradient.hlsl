struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 color_tl : register(c0);
float4 color_tr : register(c1);
float4 color_bl : register(c2);
float4 color_br : register(c3);

float4 main(ps_input_t input) : COLOR {
    return float4(lerp(
		lerp(color_tl / 255.f, color_tr / 255.f, input.uv.x),
		lerp(color_bl / 255.f, color_br / 255.f, input.uv.x),
		input.uv.y
	).rgb, input.color.a);
}