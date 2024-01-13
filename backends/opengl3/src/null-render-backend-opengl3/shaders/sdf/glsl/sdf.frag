#version 330
uniform sampler2D sampler;

in vec2 frag_uv;
in vec4 frag_color;

uniform float aa;
uniform float outline_thickness;
uniform vec4 outline_start;
uniform vec4 outline_end;

out vec4 out_color;

float stretch(float low, float high, float x) {
	return clamp((x - low) / (high - low), 0.0, 1.0);
}

void main() {
	vec4 premultiplied_os = outline_start / 255.f;
	premultiplied_os.rgb *= premultiplied_os.a;

	vec4 premultiplied_oe = outline_end / 255.f;
	premultiplied_oe.rgb *= premultiplied_oe.a;

	float distance = texture(sampler, frag_uv.st).a;
	if(distance >= 0.5f + aa) {
		out_color = frag_color;
		return;
	}
	if(distance <= outline_thickness - aa)
		discard;

	vec4 outer = vec4(0.f, 0.f, 0.f, 0.f);
	if(outline_thickness != 0.5f)
		outer = mix(premultiplied_oe, premultiplied_os, stretch(outline_thickness, 0.5f, distance));
	outer.a *= stretch(outline_thickness - aa, outline_thickness + aa, distance);

	float m = stretch(0.5f - aa, min(1.f, 0.5f + aa), distance);
	float ia = m * frag_color.a;
	float oa = (1.0 - m) * outer.a;
	float a = ia + oa;
	out_color = vec4((frag_color.rgb * ia + outer.rgb * oa) / a * a, a);
};