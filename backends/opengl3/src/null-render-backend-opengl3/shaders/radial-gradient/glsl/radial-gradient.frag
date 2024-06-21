#version 330

in vec2 frag_uv;
in vec4 frag_color;

uniform vec2 origin;
uniform vec2 radius;
uniform int stops_count;
uniform vec4 colors[16];
uniform float stops[16];

out vec4 out_color;

void main() {
    float t = length((1.f / radius) * (frag_uv - origin));

    out_color = colors[0] / 255.f;
    out_color.rgb *= out_color.a;
    for(int i = 1; i < stops_count; ++i) {
        vec4 premultiplied = colors[i] / 255.f;
        premultiplied.rgb *= premultiplied.a;
        out_color = mix(out_color, premultiplied, smoothstep(stops[i - 1], stops[i], t));
    }

    out_color *= frag_color;
}