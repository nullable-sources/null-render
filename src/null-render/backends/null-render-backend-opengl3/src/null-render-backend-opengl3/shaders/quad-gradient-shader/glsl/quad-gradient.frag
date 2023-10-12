#version 330

in vec2 frag_uv;
in vec4 frag_color;

uniform vec4 color_tl;
uniform vec4 color_tr;
uniform vec4 color_br;
uniform vec4 color_bl;

out vec4 out_color;

void main() {
	out_color = mix(
		mix(color_tl / 255.f, color_tr / 255.f, frag_uv.x),
		mix(color_bl / 255.f, color_br / 255.f, frag_uv.x),
		frag_uv.y
	) * frag_color;
}