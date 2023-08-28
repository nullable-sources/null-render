struct ps_input_t {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main(ps_input_t input) : SV_Target {
	return input.color;
}