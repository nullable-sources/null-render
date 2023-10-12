cbuffer constant_buffer : register(b0) {
	int4 outline_start, outline_end;
	float aa, outline_thickness;
};

struct ps_input_t {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source;

float stretch(float low, float high, float x) {
	return clamp((x - low) / (high - low), 0.0, 1.0);
}

float4 main(ps_input_t input) : SV_Target {
	float distance = texture_source.Sample(texture_sampler, input.uv).a;
	if(distance >= 0.5f + aa)
		return input.color;
	
	if(distance <= outline_thickness - aa)
		discard;

	float4 outer = float4(0.f, 0.f, 0.f, 0.f);
	if(outline_thickness != 0.5f) outer = lerp(outline_end / 255.f, outline_start / 255.f, stretch(outline_thickness, 0.5f, distance));
	outer.a *= stretch(outline_thickness - aa, outline_thickness + aa, distance);
	
	float m = stretch(0.5f - aa, min(1.f, 0.5f + aa), distance);	
	float ia = m * input.color.a;
	float oa = (1 - m) * outer.a;
	float a = ia + oa;
	return float4((input.color.rgb * ia + outer.rgb * oa) / a, a);
}