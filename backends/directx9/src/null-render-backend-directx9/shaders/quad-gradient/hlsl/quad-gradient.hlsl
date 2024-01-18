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
	float4 premultiplied_ctl = color_tl / 255.f;
	premultiplied_ctl.rgb *= premultiplied_ctl.a;

	float4 premultiplied_ctr = color_tr / 255.f;
	premultiplied_ctr.rgb *= premultiplied_ctr.a;

	float4 premultiplied_cbr = color_br / 255.f;
	premultiplied_cbr.rgb *= premultiplied_cbr.a;

	float4 premultiplied_cbl = color_bl / 255.f;
	premultiplied_cbl.rgb *= premultiplied_cbl.a;

	return lerp(
		lerp(premultiplied_ctl, premultiplied_ctr, input.uv.x),
		lerp(premultiplied_cbl, premultiplied_cbr, input.uv.x),
		input.uv.y
	) * input.color;
}
