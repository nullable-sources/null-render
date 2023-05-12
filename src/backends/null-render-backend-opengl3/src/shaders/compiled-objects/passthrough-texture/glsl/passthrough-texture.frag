#version 330
uniform sampler2D sampler;

in vec2 frag_uv;
in vec4 frag_color;

out vec4 out_color;

void main() {
	out_color = frag_color * texture(sampler, vec2(frag_uv.x, 1.f - frag_uv.y));
}