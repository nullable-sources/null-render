#version 330

in vec2 frag_uv;
in vec4 frag_color;

uniform float angle;
uniform int stops_count;
uniform vec4 colors[16];
uniform float stops[16];

out vec4 out_color;

void main() {
	vec2 uv = frag_uv - 0.5f;
    float t = 0.5f + length(uv) * cos(atan(-uv.y, uv.x) + angle);

    out_color = colors[0] / 255.f;
    for(int i = 1; i < stops_count; ++i) {
        out_color = mix(out_color, colors[i]  / 255.f, smoothstep(stops[i - 1], stops[i], t));
    }
}