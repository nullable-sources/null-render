struct ps_input_t {
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

sampler texture_sampler;

float iterations : register(c0);
float2 direction : register(c1);
float2 texel_size : register(c2);
float4 uv_limits : register(c3);
float4 offsets[24] : register(c4);
float4 weights[24] : register(c28);

float2 clamp_uv(float2 uv) { return clamp(uv, uv_limits.xy, uv_limits.zw); }

float4 main(ps_input_t input) : COLOR {
	float4 color = input.color * tex2D(texture_sampler, clamp_uv(input.uv));
	color *= weights[0][0];

	for(int i = 1; i < iterations; ++i) {
		float weight = ((float[24 * 4])weights)[i];

		float2 offset = texel_size * ((float[24 * 4])offsets)[i] * direction;
		color += tex2D(texture_sampler, clamp_uv(input.uv - offset)) * weight;
		color += tex2D(texture_sampler, clamp_uv(input.uv + offset)) * weight;
	}

    color.rgb *= color.a;
	return color;
}
