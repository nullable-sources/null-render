#version 330
uniform sampler2D sampler;

in vec2 frag_uv;
in vec4 frag_color;

uniform vec2 direction;
uniform vec2 texel_size;
uniform vec4 uv_limits;
uniform float offsets[96];
uniform float weights[96];
uniform int iterations;

out vec4 out_color;

vec2 clamp_uv(vec2 uv) {
    return clamp(uv, uv_limits.xy, uv_limits.zw);
}

void main() {
	out_color = frag_color * texture(sampler, clamp_uv(frag_uv));
	out_color *= weights[0];
	
	for (int i = 1; i < iterations; ++i) {
        vec2 offset = texel_size * offsets[i] * direction;
		out_color += texture(sampler, clamp_uv(frag_uv - offset)) * weights[i];
		out_color += texture(sampler, clamp_uv(frag_uv + offset)) * weights[i];
	}
}