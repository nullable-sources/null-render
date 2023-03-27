cbuffer constant_buffer : register(b0) {
    int4 color_tl, color_tr, color_bl, color_br;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(ps_input_t input) : SV_Target {
    return float4(lerp(
		lerp(color_tl / 255.f, color_tr / 255.f, input.uv.x),
		lerp(color_bl / 255.f, color_br / 255.f, input.uv.x),
		input.uv.y
	).rgb, input.color.a);
}